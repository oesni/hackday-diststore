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
		int leader_idx = -1;
		bool states[3];

	public:
		DsSelector() {
			ip_list[0] = "10.33.1.156:8090";
			ip_list[1] = "10.33.1.156:8090";
			ip_list[2] = "10.33.1.156:8090";

			int i;
			for (i = 0; i < 3; i++) {
				channels[i] = grpc::CreateChannel(ip_list[i], grpc::InsecureChannelCredentials());
				states[i] = false;
			}
		}

		~DsSelector() {
		}


		void Update(int _leader_idx, bool _states[]);

		std::shared_ptr<Channel> GetLeader();

		std::shared_ptr<Channel> GetRandomDs();
};
