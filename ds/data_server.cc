#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>

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
using distsotre::GetFileRequest;
using distsotre::GetFileReply;
using distsotre::CheckHealthRequest;
using diststore::CheckHealthReply;
using diststore::HelpRequest;
using diststore::HelpReply;

using grpc::channel;
using grpc::ClientContext;

const static std::string storage_prefix("/data/");

class Peer final {
    private:
        static int c = 0;
    public:
    int count() return c;
    const std::string peer;
    Peer(std::string p) : peer(p){c+=1}
}

class DsServiceClient;

class DataServer : public DsService::Service {
    //variable
    unsigned long logIndex = 0;
    fstream logFile;
    bool isLeader;
    std::vector<DsServiceClient> clients;

    DataServer(std::vector<Peer>peer, bool leader)
        :isLeader(leader);
    {
        logFile = fstream(storage_prefix+"log", fstream:out | fstream:in | fstream:app);
        if(!logFile.good())
        {
            std::cerr<<"cannot open log file !!"<<std::endl;
            exit(1);
        }

        for_each(peer.begin(),peer.end(),[](Peer p){
            clients.push_back(DsServiceClient(grpc::CreateChannel(p.peer, grpc::InsecureChannelCredentials())));
        });
    }

    Status PutFile(ServerContext* context, const PutFileRequest* request, PutFileReply* reply)
        override
    {
        if(!isLeader)   //if not leader
        {
            reply -> set_message("I'm not leader!!");
            return Status::OK;
        }
        ofstream file(storage_prefix + request->name(), ,ios::out);
        if(file.open())
        {
            file << request -> contents();
            file.close();
            logFile << ++logIndex<<" "<<"w"<<" "<<request->name()<<std::endl;
            //send to peer
            for_each(clients.begin(), clients.end(), [](DsServiceClient client)
                std::string reply = client.Replicate(request->name(),request->contents());
                    });
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
       if(file.open())
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

            GetFileReply reply;
            Status status = stub_->GetFile(&context, request, &reply);
            if(status.ok())
            {
                if(reply.message() == "ok")
                    return reply.contents();
                else
                    return reply.message();
            }
            else
            {
                std::cout << status.error_code() << " : "<<status.error_message() << std::endl;
                return "RPC failed";
            }
        }
};
int main()
{

    return 0;
}

