#include "Graph.h"
#include <boost/test/auto_unit_test.hpp>
#define BOOST_TEST_MODULE Graph test

BOOST_AUTO_TEST_SUITE( GraphSuite )

BOOST_AUTO_TEST_CASE( size )
{
    Graph graph("aba", "bab");
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

