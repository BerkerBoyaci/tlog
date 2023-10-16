#include <iostream>
#include "logger.h"

using namespace Log;


int main() {

	logger_w::set_log_output("log/log.txt");
	logger_w::set_log_priority(LogPriority::Trace);
	auto log = logger_w::get_instance();
	log->set_formatter(L" %m %t");


	log->log(LogPriority::Fatal, L"test deneme123");
	log->log(LogPriority::Error,L"नमस्ते दुनिया");
	log->log(LogPriority::Warning, L"Γειά σου Κόσμε");
	log->log(LogPriority::Info, L"こんにちは世界");


	return 0;
}
