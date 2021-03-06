#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpc++/grpc++.h>

#include "mgmt_svc.grpc.pb.h"
#include "ds_svc.grpc.pb.h"
#include "mgmt_svc.pb.h"
#include "ds_svc.pb.h"

using namespace std;
// client
using grpc::Channel;
using grpc::ClientContext;
// server
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
// mgmt server respondent
using diststore::MgmtService;
using diststore::GetMembershipRequest;
using diststore::GetMembershipResponse;
using diststore::DsService;
using diststore::CheckHealthRequest;
using diststore::CheckHealthReply;

string mgmtPort = "0.0.0.0:8080";
string addr[3] = {"10.33.12.102:8081" ,"10.32.25.45:8081" ,"10.32.26.128:8081" }; // port number of 0: ds1, 1: ds2, 2: ds3
string dsTable[3]; // 0: ds1, 1: ds2, 2: ds3
int dsLogTable[3];
int flag = 0; // check if leader is safe!

class DsServiceClient {
	public:
		DsServiceClient(std::shared_ptr<Channel> channel)
			: stub_(DsService::NewStub(channel)){}

		std::string CheckHealth(int dsnum){
			
			CheckHealthRequest request;

			ClientContext context;
			std::chrono::system_clock::time_point deadline = 
				std::chrono::system_clock::now() + std::chrono::seconds(1);
			context.set_deadline(deadline);

			CheckHealthReply reply;
			Status status = stub_->CheckHealth(&context, request, &reply);

			if(status.ok()){
				if(reply.message() == "ok") {
					if (dsTable[dsnum] == "F") {
						dsTable[dsnum] = "N";
						std::cout << dsnum << ": F -> N" << " : " << dsLogTable[dsnum] << std::endl;
					}
					dsLogTable[dsnum] = (int)reply.lastlogindex();
					//std::cout << dsnum << ": " << dsTable[dsnum] << " : " << dsLogTable[dsnum] << std::endl;
					return "success";
				}
			} else {
				std::cout << status.error_code() << ": "<< status.error_message() << std::endl;
				if(dsTable[dsnum] == "L")	flag =1;
				dsTable[dsnum] = "F";
				std::cout << dsnum << ": " << dsTable[dsnum] << std::endl;
				return "RPC failed";
			}

		}
	private:
		std::unique_ptr<DsService::Stub> stub_;
};

class MgmtServiceImpl final : public MgmtService::Service {
	Status GetMembership(ServerContext *context, const GetMembershipRequest *request, GetMembershipResponse *reply){
		reply->set_member1(dsTable[0]);
		reply->set_member2(dsTable[1]);
		reply->set_member3(dsTable[2]);
		std::cout << "[" << dsTable[0] << "," << dsTable[1] << "," << dsTable[2] << "]";
		return Status::OK;
	}
};

void electLeader(){
	int dsnum = 0;
	if((dsTable[dsnum] == "F" && dsTable[1]=="N") || (dsTable[1] !="F" && (dsLogTable[dsnum] < dsLogTable[1])) ){
		if(dsTable[dsnum] == "L")	dsTable[dsnum] = "N";
		dsTable[1] = "L";
		dsnum = 1;
	}
	
	if((dsTable[dsnum] == "F" && dsTable[2]=="N") || (dsTable[2] !="F" && (dsLogTable[dsnum] < dsLogTable[2])) ){
		if(dsTable[dsnum] == "L")	dsTable[dsnum] = "N";
		dsTable[2] = "L";
		dsnum = 2;
	}
}

void sysinit(){
	dsLogTable[0] = 0;
	dsLogTable[1] = 0;
	dsLogTable[2] = 0;
	
	dsTable[0] = "L";
	dsTable[1] = "N";
	dsTable[2] = "N";
}

int main(){
	int i;
	sysinit();
	electLeader();

	std::string mgmtAddr(mgmtPort); // mgmt port.
	MgmtServiceImpl service; 

	ServerBuilder builder;
	builder.AddListeningPort(mgmtAddr, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << mgmtAddr << std::endl;

	while(true){
		for (i = 0; i < 3; i++) {
			DsServiceClient client(grpc::CreateChannel(addr[i], grpc::InsecureChannelCredentials()));
			std::string reply = client.CheckHealth(i);
			//std::cout << "i:" << i << ", reply:" << reply << std::endl;
		}
		
		if(flag == 1)	electLeader();

		usleep(1*1000*1000);
	}

	server->Wait();

	return 0;
}
