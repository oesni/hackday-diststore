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
using diststore::GetFileRequest;
using diststore::GetFileReply;

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

		bool PutFile(const std::string &name, const std::string &contents) {
			PutFileRequest request;
			request.set_name(name);
			request.set_contents(contents);

			ClientContext context;
			std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(10);
			context.set_deadline(deadline);

			PutFileReply reply;
			Status status = stub_->PutFile(&context, request, &reply);
			if (!status.ok()) {
				std::cout << status.error_code() << ": " << status.error_message() << std::endl;
				return false;
			} else {
				return true;
			}
		}

		std::pair<bool, std::string> GetFile(const std::string &name) {
			GetFileRequest request;
			request.set_name(name);

			ClientContext context;
			std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(10);
			context.set_deadline(deadline);

			GetFileReply reply;
			Status status = stub_->GetFile(&context, request, &reply);
			if (!status.ok()) {
				std::cout << status.error_code() << ": " << status.error_message() << std::endl;
				return std::make_pair(false, "rpc fail");
			} else if (reply.message() == "ok") {
				return std::make_pair(true, reply.contents());
			} else {
				return std::make_pair(false, reply.message());
			}
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
			std::string mbr = dsSelector->Update(leader_idx, states);
			std::cout << "mbr updated: " << mbr << std::endl;
		} else {
			std::cerr << "fail to get membership from mgmt. err:" << ret << std::endl;
		}
		usleep(1 * 1000 * 1000);
	}
}

bool PutFile(DsSelector *dsSelector, const std::string &name, const std::string &contents) {
	std::shared_ptr<Channel> channel = dsSelector->GetLeader();
	if (!channel) {
		std::cerr << "there is no leader" << std::endl;
		return false;
	} else {
		DsServiceClient client(channel);
		return client.PutFile("foo", "hello");
	}
}

std::pair<bool, std::string> GetFile(DsSelector *dsSelector, const std::string &name) {
	std::shared_ptr<Channel> channel = dsSelector->GetRandomDs();
	if (!channel) {
		return std::make_pair(false, "there is no ds alive");
	} else {
		DsServiceClient dsClient(channel);
		return dsClient.GetFile("foo");
	}
}

std::pair<bool, std::string> GetFileWithIdx(DsSelector *dsSelector, const std::string &name, int idx) {
	std::shared_ptr<Channel> channel = dsSelector->GetDs(idx);
	if (!channel) {
		return std::make_pair(false, "there is no ds alive");
	} else {
		DsServiceClient dsClient(channel);
		return dsClient.GetFile("foo");
	}
}

void Putter(DsSelector *dsSelector, std::string name, std::string contents) {
	bool ret = PutFile(dsSelector, name, contents);
	std::cout << "Putter. put(" << name << ", " << contents << "). ret:" << ret << std::endl;
}



void pleaseEnter() {
	std::cout << "Press the ENTER key" << std::endl;
	std::cin.get();
}

int main(int argc, char **argv) {
	bool ret;
	int i, c;
	DsSelector dsSelector;
	//MgmtServiceClient client(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));
	//std::thread mbrUpdater(&MembershipUpdater, &client, &dsSelector);
	
	std::cout << "[TEST] replication" << std::endl;
	std::cout << "ready to put?" << std::endl;
	pleaseEnter();
	ret = PutFile(&dsSelector, "foo", "hello");
	std::cout << "put(foo, hello) : " << ret << std::endl;
	std::cout << "ready to get?" << std::endl;
	pleaseEnter();
	for (i = 0; i < 3; i++) {
		std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "foo", i);
		std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
	}

	std::cout << "[TEST] overwrite" << std::endl;
	std::cout << "ready to put?" << std::endl;
	pleaseEnter();
	ret = PutFile(&dsSelector, "foo", "bye");
	std::cout << "put(foo, bye) : " << ret << std::endl;
	std::cout << "ready to get?" << std::endl;
	pleaseEnter();
	for (i = 0; i < 3; i++) {
		std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "foo", i);
		std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
	}

	std::cout << "[TEST] multithread put" << std::endl;
	std::cout << "ready to put?" << std::endl;
	pleaseEnter();
	std::thread putter1(&Putter, &dsSelector, "a.jpg", "hello1");
	std::thread putter2(&Putter, &dsSelector, "a.jpg", "hello2");
	putter1.join();
	putter2.join();
	std::cout << "ready to get?" << std::endl;
	pleaseEnter();
	for (i = 0; i < 3; i++) {
		std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "a.jpg", i);
		std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
	}

	std::cout << "[TEST] kill 1 follower" << std::endl;
	std::cout << "kill 1 follower and..." << std::endl;
	pleaseEnter();
	for (c = 0; c < 10; c++) {
		for (i = 0; i < 3; i++) {
			std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "a.jpg", i);
			std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
		}
		usleep(1*1000*1000);
	}
	ret = PutFile(&dsSelector, "c1.txt", "c1c1c1");
	std::cout << "put(c1.txt, c1c1c1) : " << ret << std::endl;
	ret = PutFile(&dsSelector, "c2.txt", "c2c2c2");
	std::cout << "put(c2.txt, c2c2c2) : " << ret << std::endl;
	ret = PutFile(&dsSelector, "c3.txt", "c3c3c3");
	std::cout << "put(c3.txt, c3c3c3) : " << ret << std::endl;
	for (i = 0; i < 3; i++) {
		std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "c3.txt", i);
		std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
	}

	std::cout << "[TEST] recovery 1 follower" << std::endl;
	std::cout << "revive 1 follower and..." << std::endl;
	pleaseEnter();
	for (c = 0; c < 10; c++) {
		for (i = 0; i < 3; i++) {
			std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "c3.txt", i);
			std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
		}
		usleep(1*1000*1000);
	}

	std::cout << "[TEST] kill leader" << std::endl;
	std::cout << "kill leader and..." << std::endl;
	pleaseEnter();
	for (c = 0; c < 10; c++) {
		ret = PutFile(&dsSelector, "love", "love is pain");
		std::cout << "put(love, love is pain) : " << ret << std::endl;
		if (ret) {
			break;
		}
		usleep(1*1000*1000);
	}
	for (i = 0; i < 3; i++) {
		std::pair<bool, std::string> p = GetFileWithIdx(&dsSelector, "love", i);
		std::cout << "get-" << i << ": " << p.first << ", " << p.second << std::endl;
	}

	//mbrUpdater.join();
	return 0;
}
