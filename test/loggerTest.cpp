


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
