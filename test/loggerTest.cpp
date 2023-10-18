


#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "logger.h"

using namespace Log;

BOOST_AUTO_TEST_CASE(MyTestCase)
{

	//BOOST_CHECK(x != 0.0f);
	//BOOST_CHECK_EQUAL((int)x, 9);
	//BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
	//BOOST_CHECK_EQUAL((int)x, 9);

    BOOST_CHECK_EQUAL(1_kb, 1024);
    BOOST_CHECK_EQUAL(1_mb, 1024 * 1024);
    BOOST_CHECK_EQUAL(1_gb, 1024 * 1024 * 1024);
}


BOOST_AUTO_TEST_CASE(instanceTest)
{
    BOOST_CHECK_EQUAL(logger_c::get_instance(), logger_c::get_instance());
}

BOOST_AUTO_TEST_CASE(FilePathTest)
{
	logger_c::set_log_output("log.txt");
    BOOST_CHECK_EQUAL(	logger_c::get_log_output(), "log.txt");
}

BOOST_AUTO_TEST_CASE(LogPriorityTest)
{
	logger_c::set_log_priority(LogPriority::Fatal);
    BOOST_CHECK(logger_c::get_log_priority() ==  LogPriority::Fatal);
}

BOOST_AUTO_TEST_CASE(FormatterTest)
{
	logger_c::set_formatter("%m %t");
    BOOST_CHECK_EQUAL(logger_c::get_formatter(), "%m %t");
}

BOOST_AUTO_TEST_CASE(FileLimitTest)
{
	logger_c::set_file_limit(1_kb);
    BOOST_CHECK_EQUAL(logger_c::get_file_limit(), 1_kb);
}

BOOST_AUTO_TEST_CASE(LogFormatFileTest)
{
	logger_c::set_log_format();
    BOOST_CHECK( logger_c::get_log_format() == LogOutput::File);
}


BOOST_AUTO_TEST_CASE(LogFormatConsoleTest)
{
	logger_c::set_log_output("");
	logger_c::set_log_format();
    BOOST_CHECK( logger_c::get_log_format() == LogOutput::Console);
}