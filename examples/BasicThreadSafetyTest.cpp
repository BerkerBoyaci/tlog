#include <iostream>
#include <vector>
#include <thread>
#include "logger.h"

using namespace Log;


void test(std::weak_ptr<logger<char>> log_ptr, size_t i) {

	auto t_sharedlockptr = log_ptr.lock();

	if (t_sharedlockptr) {
		for(size_t i = 0 ; i < 100'000; i++)
			t_sharedlockptr->log(LogPriority::Fatal, i);
	}
}


int main() {
	logger_c::set_log_output("log/log.txt");
	logger_c::set_log_priority(LogPriority::Fatal);
	auto log = logger_c::get_instance();
	const size_t sizeThread = std::thread::hardware_concurrency();
	std::vector<std::thread> t_vec;

	for (size_t i = 0; i < sizeThread; i++) {
		t_vec.push_back(std::thread(test, log, i));
	}

	for (auto& th : t_vec)
		th.join();

	return 0;
}
