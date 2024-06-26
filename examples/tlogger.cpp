#include <iostream>
#include "logger.hpp"

using namespace Log;

#define WLOG_TEST 0


int main() {

	//Log::Formatter<wchar_t> fd;
	//fd.getFormatter(L"MESSAGE:%m\tTIME:%t\tLINE:%l\tFUNC:%f",formatPattern::XML);
	//auto ret = fd.format(__LINE__,_T(__FILE__) , L"berker", L"boyaci", L"123");
	//std::wcout << ret << "\n";
#if WLOG_TEST
	// Usage Samples
	logger_w::set_log_output(L"log.txt");
	logger_w::set_log_priority(LogPriority::Debug);
	auto log = logger_w::get_instance();
	log->set_formatter(L"MESSAGE:%m\tLINE:%l\tTIME:%t\tFUNC:%f");

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, 'A', L" porta tincidunt lectus turpis id sem. Integer et est neque.", L"Vestibulum at maximus ante. ");
	auto finish1 = std::chrono::high_resolution_clock::now();

	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	auto finish2 = std::chrono::high_resolution_clock::now();
	auto start3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", L" vestibulum nibh.", L"Nulla in auctor leo. ");
	auto finish3 = std::chrono::high_resolution_clock::now();
	auto start5 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	auto finish5 = std::chrono::high_resolution_clock::now();
	auto start6 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", 2.712, 'z', L" vestibulum nibh.", L"Nulla in auctor leo. ");
	auto finish6 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::chrono::duration<double> elapsed3 = finish3 - start3;

	std::chrono::duration<double> elapsed5 = finish5 - start5;
	std::chrono::duration<double> elapsed6 = finish6 - start6;

	std::cout << "Error : " << elapsed1.count() << "\nWarning : " << elapsed2.count() <<
		"\nInfo : " << elapsed3.count() << "\nWarning" << elapsed5.count() <<
		"\nInfo : " << elapsed6.count() << "\n";


#else 
	/* Create log ->*/

	logger_c::log_output_set("log.txt");
	logger_c::log_priority_set(LogPriority::Debug);
	auto log = logger_c::get_instance();
	log->formatter_set("%m %l %t %f");
	log->file_limit_set(100);

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	auto finish1 = std::chrono::high_resolution_clock::now();
	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish2 = std::chrono::high_resolution_clock::now();
	auto start3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, __FILE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish3 = std::chrono::high_resolution_clock::now();

	auto start5 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish5 = std::chrono::high_resolution_clock::now();
	auto start6 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish6 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::chrono::duration<double> elapsed3 = finish3 - start3;

	std::chrono::duration<double> elapsed5 = finish5 - start5;
	std::chrono::duration<double> elapsed6 = finish6 - start6;

	std::cout << "Error : " << elapsed1.count() << "\nWarning : " <<
		elapsed2.count() << "\nInfo : " << elapsed3.count() << "\n" <<
		elapsed5.count() << "\nInfo : " << elapsed6.count() << "\n";
#endif


	return 0;
}
