#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>


#include <grpc++/grpc++.h>

#include "../proto/ds_svc.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

// client
using grpc::ClientReader;

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
const static std::string logFilename("/data/log");
unsigned long logIndex = 0;

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

		std::string Help() {
			HelpRequest request;
			request.set_lastlogindex(logIndex);

			ClientContext context;
			std::chrono::system_clock::time_point deadline = 
				std::chrono::system_clock::now() + std::chrono::seconds(10);
			context.set_deadline(deadline);

			HelpReply h;
			std::unique_ptr<ClientReader<HelpReply>> reader(stub_->Help(&context, request));
			
    		fstream logFile;
			logFile.open(logFilename, fstream::out | fstream::in | fstream::app);
			if(!logFile.good()){
				std::cerr<<"cannot open log file!!!"<<std::endl;
				exit(1);
			}
			while(reader->Read(&h)) {
				// action like put file
				ofstream file (storage_prefix + h.logname(), ios::out);
				if(file.is_open()){
					file<<h.logcontent();
					file.close();
				}else{
					return "failed";
				}
            	logFile << ++logIndex<<" "<<"w"<<" "<<h.logname()<<std::endl;
			}
			logFile.close();
			Status status = reader->Finish();
			return "ok";
		}
};

class DataServer : public DsService::Service {
    public:
    DataServer(std::vector<Peer>peer, bool leader)
        :isLeader(leader)
    {
        logFile.open(logFilename, fstream::out | fstream::in | fstream::app);
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
    fstream logFile;
    bool isLeader;
    std::vector<DsServiceClient> clients;

    
    
    Status PutFile(ServerContext* context, const PutFileRequest* request, PutFileReply* reply)
        override
    {
        std::cout<<"PutFile!"<<std::endl;
        //if(!isLeader)   //if not leader
        //{
         //   reply -> set_message("I'm not leader!!");
         //   return Status::OK;
        //}
        ofstream file(storage_prefix + request->name() ,ios::out);
        if(file.is_open())
        {
            file << request -> contents();
            file.close();
            logFile << ++logIndex<<" "<<"w"<<" "<<request->name()<<std::endl;
            //send to peer
            std::mutex mtx;
            std::vector<std::thread> threads;
            int count = 0;
            auto client = clients.begin();
         
            while(client != clients.end())
            {
                threads.push_back(std::thread([this,&mtx,&count,client,request]() {
                    std::string reply = client -> Replicate(request->name(),request->contents());
                    if(reply == "ok")
                    {
                        mtx.lock();
                        count++;
                        mtx.unlock();
                    }
                            }));
                client++;
            }

            for(std::thread& t : threads)
                t.join();
            if(count)
                reply -> set_message("ok");
            else
                reply -> set_message("fail to save file");
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
            file.close();
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

    Status Help(ServerContext* context, const HelpRequest* request, ServerWriter<HelpReply>* reply)
        override
    {
        //do something
		int lastIndex = request->lastlogindex();
		
		char *logfilename;
		strcat(logfilename, logFilename.c_str());
        
		FILE* log = fopen(logfilename, "r");
		logFile.open(logFilename, fstream::out | fstream::in | fstream::app);
		std::string line;
		int num;
		//strtok...
		while(true){
			fscanf(log, "%d", &num );
			getline(logFile, line);
			if(num == lastIndex){
				break;
			}
		}
		//logFile form: logIndex<<" "<<"w"<<" "<<request->name()<<std::endl;
		HelpReply h;
		while(true){
			fscanf(log, "%d", &num );
			getline(logFile, line);
			if(!line.empty()){
				line = line.substr(3);
	
				ifstream file(storage_prefix + line, ios::in);
				std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				file.close();
				h.set_lognum(num);
				h.set_logname(line);
				h.set_logcontent(contents);
				reply->Write(h);
			}else{
				cout<<"Scan all the log file"<<std::endl;
				break;
			}
		}
		logFile.close();
		fclose(log);

        return Status::OK;
    } /// I'm not sure.. if it will work...

};   

//get log index whenever system dies
void getlogIndex(){
	fstream logFile;
	std::string line;

    logFile.open(logFilename, fstream::out | fstream::in | fstream::app);
	logIndex = -1;	
	while(true){
		getline(logFile, line);
		if(!line.empty()){
			logIndex ++;
		}else {
			break;
		}
	}
	logFile.close();
}

int main(int argc, char** argv)
{
    std::string addr(argv[1]);
    std::vector<Peer> p;
    bool isLeader = (argv[argc-1][0] == 'L' ? true : false) ;
    for(int i=2; i<argc-1; i++)
    {
        p.push_back(Peer(std::string(argv[i])));
    }
	getlogIndex();

	if(logIndex !=0 ){
		// call Client Help;
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

