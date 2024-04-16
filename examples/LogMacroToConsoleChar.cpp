#include <iostream>
#include "logger.hpp"

using namespace Log;

int main() {

	log_set_format(" %m %t ");

	log_set_priority(56);
	log_quiet();

	log_fatal(__LINE__, __FILE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	log_error(__LINE__, __FILE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
    log_warn(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	log_info(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	log_ver(__LINE__, __FILE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	log_debug(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	log_trace(__LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");

	return 0;
}
