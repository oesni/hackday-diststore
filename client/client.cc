#include <iostream>
#include <thread>
#include <fstream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpc++/grpc++.h>

#include "mgmt_svc.grpc.pb.h"
#include "ds_svc.grpc.pb.h"
#include "mgmt_svc.pb.h"
#include "ds_svc.pb.h"

#include "ds_selector.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using diststore::MgmtService;
using diststore::DsService;
using diststore::GetMembershipRequest;
using diststore::GetMembershipResponse;
using diststore::PutFileRequest;
using diststore::PutFileReply;

class MgmtServiceClient {
	public:
		MgmtServiceClient(std::shared_ptr<Channel> channel)
			: stub_(MgmtService::NewStub(channel)) {}

		void parseMembership(std::string val, bool *is_leader, bool *states) {
			*is_leader = false;
			if (val == "L") {
				*is_leader = true;
				*states = true;
			} else if (val == "N") {
				*states = true;
			} else {
				*states = false;
			}
		}

		void parseMemberships(GetMembershipResponse *resp, int *leader_idx, bool states[]) {
			bool is_leader;
			parseMembership(resp->member1(), &is_leader, &states[0]);
			if (is_leader) {
				*leader_idx = 0;
			}
			parseMembership(resp->member2(), &is_leader, &states[1]);
			if (is_leader) {
				*leader_idx = 1;
			}
			parseMembership(resp->member3(), &is_leader, &states[2]);
			if (is_leader) {
				*leader_idx = 2;
			}
		}

		std::string GetMembership(int *leader_idx, bool states[]) {
			GetMembershipRequest request;

			ClientContext context;
			std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(1);
			context.set_deadline(deadline);

			GetMembershipResponse resp;
			Status status = stub_->GetMembership(&context, request, &resp);
			if (!status.ok()) {
				return "rpc fail";
			}
			parseMemberships(&resp, leader_idx, states);	
			return "ok";
		}

	private:
		std::unique_ptr<MgmtService::Stub> stub_;
};

class DsServiceClient {
	public:
		DsServiceClient(std::shared_ptr<Channel> channel)
			: stub_(DsService::NewStub(channel)) {}

		std::string PutFile(std::string name, std::string contents) {
			PutFileRequest request;

			ClientContext context;
			std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(1);
			context.set_deadline(deadline);

			PutFileReply reply;
			Status status = stub_->PutFile(&context, request, &reply);
			if (!status.ok()) {
				return "rpc fail";
			}
			return "ok";
		}

	private:
		std::unique_ptr<DsService::Stub> stub_;
};

void MembershipUpdater(MgmtServiceClient *client, DsSelector *dsSelector) {
	while (true) {
		int leader_idx;
		bool states[3];
		std::string ret = client->GetMembership(&leader_idx, states);
		if (ret == "ok") {
			dsSelector->Update(leader_idx, states);
			std::cout << "mbr updated" << std::endl;
		} else {
			std::cerr << "fail to get membership from mgmt. err:" << ret << std::endl;
		}
		usleep(1 * 1000 * 1000);
	}
}

int main(int argc, char **argv) {
	DsSelector dsSelector;
	MgmtServiceClient client(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));

	std::thread mbrUpdater(&MembershipUpdater, &client, &dsSelector);
	
	/*
	int leader_idx;
	bool states[3];
	std::cout << "leader_idx:" << leader_idx << std::endl;
	std::cout << states[0] << ", " << states[1] << ", " << states[2] << std::endl;
	*/

	std::shared_ptr<Channel> channel = dsSelector.GetRandomDs();
	if (channel) {
		DsServiceClient dsClient(channel);
		std::string ret = dsClient.PutFile("foo", "hello");
		std::cout << "return random " << ret << std::endl;
	} else {
		std::cerr << "there is no ds alive" << std::endl;
	}

	channel = dsSelector.GetLeader();
	if (!channel) {
		std::cerr << "there is no leader" << std::endl;
	} else {
		DsServiceClient dsClient(channel);
		std::string ret = dsClient.PutFile("foo", "hello");
		std::cout << "return " << ret << std::endl;
	}

	mbrUpdater.join();
	return 0;
}
