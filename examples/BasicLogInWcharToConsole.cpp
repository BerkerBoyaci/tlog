#include <iostream>
#include "Logger.h"

using namespace Log;

int main() {

	LoggerW::setLogPriority(LogPriority::Verbose);
	auto log = LoggerW::getInstance();
	log->setFormatter(L" %t %m");

	log->log(LogPriority::Quiet);
	log->log(LogPriority::Fatal, __LINE__, _T(__FILE__), L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", L"Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");


	return 0;
}
github 