#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "mgmt_svc.grpc.pb.h"
#include "ds_svc.grpc.pb.h"
#include "mgmt_svc.pb.h"
#include "ds_svc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using diststore::MgmtService;
using diststore::DsService;
using diststore::GetMembershipRequest;
using diststore::GetMembershipResponse;

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

/*
class DsServiceClient {

}
*/


int main(int argc, char **argv) {
	MgmtServiceClient client(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));
	
	int leader_idx;
	bool states[3];
	std::string ret = client.GetMembership(&leader_idx, states);
	std::cout << "leader_idx:" << leader_idx << std::endl;
	std::cout << states[0] << ", " << states[1] << ", " << states[2] << std::endl;
	return 0;
}
