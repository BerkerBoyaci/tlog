#include <iostream>
#include <string_view>
#include "logger.hpp"

using namespace Log;



int main()
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

	log_set_output("Log//log.txt");
	log_set_priority(48);
	log_quiet();
	log_fatal(__LINE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit."sv, "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  "sv);
	log_error(__LINE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	log_warn(__LINE__, "Nunc a varius purus. : "sv, 3.14159, "Proin dictum erat sit amet posuere tristique. "sv, "Fusce a arcu rhoncus, "sv);
	log_info(__LINE__, "Nunc a varius purus. : "sv, 3.14159, "Proin dictum erat sit amet posuere tristique. "sv, "Fusce a arcu rhoncus, "sv);
	log_ver(__LINE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. "sv);
	log_debug(__LINE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. "s, "Fusce a arcu rhoncus, "sv);
	log_trace(__LINE__, " mattis dolor id,  : "sv, 2.712, 'a', " vestibulum nibh."sv, "Nulla in auctor leo. "sv);

	return 0;
}
