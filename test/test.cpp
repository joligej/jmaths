#define BOOST_TEST_MODULE jmaths_unit_test
#include <boost/test/unit_test.hpp>

#include "all.hpp"

BOOST_AUTO_TEST_CASE(FailTest) {
    BOOST_CHECK_EQUAL(2, 3);
}

BOOST_AUTO_TEST_CASE(PassTest) {
    BOOST_CHECK_EQUAL(9, 3);
}
