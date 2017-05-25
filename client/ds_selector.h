#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpc++/grpc++.h>

using grpc::Channel;

class DsSelector {
	private:
		std::mutex mtx;
		std::shared_ptr<Channel> channels[3];
		std::string ip_list[3];
		int leader_idx = 0;
		bool states[3];

	public:
		DsSelector() {
			ip_list[0] = "127.0.0.1:8081";
			ip_list[1] = "220.230.122.199:8081";
			ip_list[2] = "220.230.122.199:8081";

			int i;
			for (i = 0; i < 3; i++) {
				channels[i] = grpc::CreateChannel(ip_list[i], grpc::InsecureChannelCredentials());
				states[i] = true;
			}
		}

		~DsSelector() {
		}


		std::string Update(int _leader_idx, bool _states[]);

		std::shared_ptr<Channel> GetLeader();

		std::shared_ptr<Channel> GetRandomDs();
		std::shared_ptr<Channel> GetDs(int idx);
};
