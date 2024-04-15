#include <iostream>
#include "logger.hpp"

using namespace Log;

int main() {

	logger<char>::set_log_priority(LogPriority::Trace);
	auto log = logger<char>::get_instance();
	log->set_formatter("%m %t");
	log->log(LogPriority::Quiet);

	log->log(LogPriority::Quiet);
	log->log(LogPriority::Fatal, __LINE__, __FILE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	log->log(LogPriority::Warning, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	log->log(LogPriority::Info, __LINE__, __FILE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	log->log(LogPriority::Verbose, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	log->log(LogPriority::Debug, __LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");
	log->log(LogPriority::Trace, __LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");


	return 0;
}
