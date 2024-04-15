#define BOOST_TEST_MODULE MyTest
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "formatter.hpp"

using namespace Log;

BOOST_AUTO_TEST_CASE(formatterSetGetTestC)
{

	//BOOST_CHECK(x != 0.0f);
	//BOOST_CHECK_EQUAL((int)x, 9);
	//BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
	//BOOST_CHECK_EQUAL((int)x, 9);

    formatter<char> format;

    format.set_formatter("%t %m");
    BOOST_CHECK(format.get_formatter() == "%t %m");
}

BOOST_AUTO_TEST_CASE(formatterTestC)
{

    formatter<char> format;

    format.set_formatter("%m");
    std::cout << format.format("lorem ipsum")<< "\n";
    BOOST_CHECK_EQUAL(format.format("lorem ipsum"), "lorem ipsum ");
}

BOOST_AUTO_TEST_CASE(formatterSetGetTestW)
{
    formatter<wchar_t> format;

    format.set_formatter(L"%t %m");
    BOOST_CHECK(format.get_formatter() == L"%t %m");
}

BOOST_AUTO_TEST_CASE(formatterTestW)
{

    formatter<wchar_t> format;

    format.set_formatter(L"%m");
    BOOST_CHECK(format.format(L"lorem ipsum") == L"lorem ipsum ");
}