#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#include "ds_selector.h"

#include <grpc++/grpc++.h>

using grpc::Channel;

std::string DsSelector::Update(int _leader_idx, bool _states[]) {
	std::stringstream ss;
	mtx.lock();
	leader_idx = _leader_idx;
	ss << "leader_idx:" << leader_idx << ", states:[";
	int i;
	for (i = 0; i < 3; i++) {
		states[i] = _states[i];
		if (i == leader_idx) {
			ss << "L";
		} else if (states[i]) {
			ss << "N";
		} else {
			ss << "F";
		}

		if (i < 2) {
			ss << ",";
		} else {
			ss << "]";
		}
	}
	mtx.unlock();
	return ss.str();
}

std::shared_ptr<Channel> DsSelector::GetLeader() {
	if (leader_idx == -1) {
		return NULL;
	}
	mtx.lock();
	int idx = leader_idx;
	mtx.unlock();
	return channels[idx];
}

std::shared_ptr<Channel> DsSelector::GetRandomDs() {

	int idx_list[3];
	int idx_list_size = 0;

	mtx.lock();
	int i;
	for (i = 0; i < 3; i++) {
		if (states[i]) {
			idx_list[idx_list_size++] = i;
		}
	}
	mtx.unlock();

	if (idx_list_size == 0) {
		return NULL;
	}

	srand((unsigned int) time(NULL));
	return channels[idx_list[rand() % idx_list_size]];
}

std::shared_ptr<Channel> DsSelector::GetDs(int idx) {
	mtx.lock();
	bool alive = states[idx];
	mtx.unlock();
	if (alive) {
		return channels[idx];
	} else {
		return NULL;
	}
}
