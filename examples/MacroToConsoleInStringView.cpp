#include <iostream>
#include <string_view>
#include "Logger.h"

using namespace Log;



int main()
{
    using namespace std::string_literals;      // access the s suffix
    using namespace std::string_view_literals; // access the sv suffix	LOG_SET_FORMAT_W(L"%m %t");

	LOG_SET_PRIORITY_W(48);
	LOG_QUIET();
	LOG_FATAL_C(__LINE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit."sv, "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  "sv);
	LOG_ERROR_C(__LINE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	LOG_WARNING_C(__LINE__, "Nunc a varius purus. : "sv, 3.14159, "Proin dictum erat sit amet posuere tristique. "sv, "Fusce a arcu rhoncus, "sv);
	LOG_INFO_C(__LINE__, "Nunc a varius purus. : "sv, 3.14159, "Proin dictum erat sit amet posuere tristique. "sv, "Fusce a arcu rhoncus, "sv);
	LOG_VERBOSE_C(__LINE__, " mattis dolor id,  : "sv, " vestibulum nibh.", "Nulla in auctor leo. "sv);
	LOG_DEBUG_C(__LINE__, "Nunc a varius purus. : "sv, 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, "sv);
	LOG_TRACE_C(__LINE__, " mattis dolor id,  : "sv, 2.712, 'a', " vestibulum nibh."sv, "Nulla in auctor leo. "sv);

	return 0;
}
