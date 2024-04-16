#include <iostream>
#include "logger.hpp"

using namespace Log;


int main() {

	log_set_format(L"%m %t");
	log_set_priority(LogPriority::Trace);
	log_quiet();
	log_fatal(__LINE__, L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", L"Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	log_error(__LINE__, L"tellus felis condimentum odio, : ", 1, 'A', L"porta tincidunt lectus turpis id sem. Integer et est neque.", L"Vestibulum at maximus ante. ");
	log_warn(__LINE__, L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	log_info(__LINE__, L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	log_ver(__LINE__, L" mattis dolor id,  : ", L" vestibulum nibh.", "Nulla in auctor leo. ");
	log_debug(__LINE__, L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	log_trace(__LINE__, L" mattis dolor id,  : ", 2.712, 'a', L" vestibulum nibh.", L"Nulla in auctor leo. ");

	return 0;
}
