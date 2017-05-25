#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <memory>

#include <grpc++/grpc++.h>

#include "../proto/ds_svc.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using diststore::DsService;
using diststore::PutFileRequest;
using diststore::PutFileReply;
using diststore::GetFileRequest;
using diststore::GetFileReply;
using diststore::CheckHealthRequest;
using diststore::CheckHealthReply;
using diststore::HelpRequest;
using diststore::HelpReply;

using grpc::Channel;
using grpc::ClientContext;

const static std::string storage_prefix("/data/");

class Peer final {
    private:
       // static int c;
    public:
    //int count() 
    //{
    //    return c;
    //}
    const std::string peer;
    Peer(std::string p) : peer(p){/*c+=1;*/}
};

class DsServiceClient {
    private:
        std::unique_ptr<DsService::Stub> stub_;
    public:
        DsServiceClient(std::shared_ptr<Channel> channel)
            :stub_(DsService::NewStub(channel)) {}

        std::string Replicate(const std::string& name, const std::string& contents)
        {
            PutFileRequest request;
            request.set_name(name);
            request.set_contents(contents);

            ClientContext context;
            std::chrono::system_clock::time_point deadline =
                std::chrono::system_clock::now() + std::chrono::seconds(5);
            context.set_deadline(deadline);

            PutFileReply reply;
            Status status = stub_->PutFile(&context, request, &reply);
            if(status.ok())
            {
                return reply.message();
            }
            else
            {
                std::cout << status.error_code() << " : "<<status.error_message() << std::endl;
                return "RPC failed";
            }
        }
};

class DataServer : public DsService::Service {
    public:
    DataServer(std::vector<Peer>peer, bool leader)
        :isLeader(leader)
    {
        logFile.open(storage_prefix+"log", fstream::out | fstream::in | fstream::app);
        if(!logFile.good())
        {
            std::cerr<<"cannot open log file !!"<<std::endl;
            exit(1);
        }

        for_each(peer.begin(),peer.end(),[this](Peer p){
            clients.push_back(DsServiceClient(grpc::CreateChannel(p.peer, grpc::InsecureChannelCredentials())));
        });
    }
//variable
    unsigned long logIndex = 0;
    fstream logFile;
    bool isLeader;
    std::vector<DsServiceClient> clients;

    
    Status PutFile(ServerContext* context, const PutFileRequest* request, PutFileReply* reply)
        override
    {
        if(!isLeader)   //if not leader
        {
            reply -> set_message("I'm not leader!!");
            return Status::OK;
        }
        ofstream file(storage_prefix + request->name() ,ios::out);
        if(file.is_open())
        {
            file << request -> contents();
            file.close();
            logFile << ++logIndex<<" "<<"w"<<" "<<request->name()<<std::endl;
            //send to peer
            //
            int count = 0;
            auto client = clients.begin();
            while(client != clients.end())
            {
                std::string reply = client->Replicate(request->name(), request->contents());
                client++;
                count +=1;
            }
            reply -> set_message("ok");
        }
        else
        {
            reply -> set_message("fail to open file");
        }
        return Status::OK;
    }

    Status GetFile(ServerContext* context, const GetFileRequest* request, GetFileReply* reply)
        override
    {
       ifstream file(storage_prefix + request->name(), ios::in);
       if(file.is_open())
       {
            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            reply -> set_message("ok");
            reply -> set_contents(contents);
       }
       else
       {
           reply -> set_message("fail to open file");
       }
        return Status::OK;
    }

    Status CheckHealth(ServerContext* context, const CheckHealthRequest* request, CheckHealthReply* reply)
        override
    {
        reply -> set_message("ok");
        reply -> set_lastlogindex(logIndex);

        return Status::OK;
    }

    Status Help(ServerContext* context, const HelpRequest* request, HelpReply* reply)
        override
    {
        //do something

        return Status::OK;
    }

};

;
int main(int argc, char** argv)
{
    std::string addr(argv[1]);
    std::vector<Peer> p;
    bool isLeader = (argv[argc-1][0] == 'L' ? true : false) ;
    for(int i=2; i<argc-1; i++)
    {
        p.push_back(Peer(std::string(argv[i])));
    }

    DataServer service(p,isLeader);

    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout<<"Server listening on " << addr << std::endl;

    server->Wait();

    return 0;
}

