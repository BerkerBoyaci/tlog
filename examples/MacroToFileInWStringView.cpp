#include <iostream>
#include <string_view>
#include "logger.hpp"

using namespace Log;



int main()
{
    using namespace std::string_literals;      // access the s suffix
    using namespace std::string_view_literals; // access the sv suffix	LOG_SET_FORMAT_W(L"%m %t");

	log_set_output("basicLog\\log\\log.txt");
	log_set_format(L"%m %t");

	log_set_priority(LogPriority::Info);
	log_quiet();
    std::wstring_view sv{L"123",3};
	log_fatal(__LINE__);
	log_error(__LINE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	log_warn(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. "sv, L"Fusce a arcu rhoncus, "sv);
	log_info(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. "sv, L"Fusce a arcu rhoncus, "sv);
	log_ver(__LINE__, L" mattis dolor id,  : "sv, L" vestibulum nibh.", L"Nulla in auctor leo. "sv);
	log_debug(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, "sv);
	log_trace(__LINE__, L" mattis dolor id,  : "sv, 2.712, 'a', L" vestibulum nibh."sv, L"Nulla in auctor leo. "sv);

	return 0;
}
