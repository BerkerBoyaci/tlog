#include <iostream>
#include <string_view>
#include "logger.h"

using namespace Log;



int main()
{
    using namespace std::string_literals;      // access the s suffix
    using namespace std::string_view_literals; // access the sv suffix	LOG_SET_FORMAT_W(L"%m %t");

	LOG_SET_OUTPUT_W("basicLog\\log\\log.txt");
	LOG_SET_FORMAT_W(L"%m %t");

	LOG_SET_PRIORITY_W(LogPriority::Info);
	LOG_QUIET();
    std::wstring_view sv{L"123",3};
	LOG_FATAL_W(__LINE__);
	LOG_ERROR_W(__LINE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	LOG_WARNING_W(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. "sv, L"Fusce a arcu rhoncus, "sv);
	LOG_INFO_W(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. "sv, L"Fusce a arcu rhoncus, "sv);
	LOG_VERBOSE_W(__LINE__, L" mattis dolor id,  : "sv, L" vestibulum nibh.", L"Nulla in auctor leo. "sv);
	LOG_DEBUG_W(__LINE__, L"Nunc a varius purus. : "sv, 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, "sv);
	LOG_TRACE_W(__LINE__, L" mattis dolor id,  : "sv, 2.712, 'a', L" vestibulum nibh."sv, L"Nulla in auctor leo. "sv);

	return 0;
}
