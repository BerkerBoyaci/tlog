#include <iostream>
#include "logger.hpp"
#include <cassert>
using namespace Log;


int main() {
	
	logger_w::log_priority_set(LogPriority::Verbose);
	auto log = logger_w::get_instance();
	log->formatter_set(L" %t %m");
	//log->setFileLimit(2_mb);

	log->log(LogPriority::Quiet);
	log->log(LogPriority::Fatal, __LINE__, _T(__FILE__), L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", L"Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	

	return 0;
}