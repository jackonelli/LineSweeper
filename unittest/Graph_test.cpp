#include "Graph.h"
#include <boost/test/auto_unit_test.hpp>
#define BOOST_TEST_MODULE Graph test

BOOST_AUTO_TEST_SUITE( GraphSuite )

BOOST_AUTO_TEST_CASE( NumberOfNodes )
{
 Graph graph;
 BOOST_CHECK_EQUAL(graph.GetNumberOfNodes(), 0);
 graph.AddNode(1.0, 0.0);
 BOOST_CHECK_EQUAL(graph.GetNumberOfNodes(), 1);
}

BOOST_AUTO_TEST_CASE( ValidateEdge )
{
 Graph graph;

 // Different nodes, ok
 graph.AddNode(0.0, 0.0);
 graph.AddNode(1.0, 0.0);
 BOOST_CHECK(graph.ValidateEdge(0, 1));

 // Same nodes, not ok
 BOOST_CHECK(!graph.ValidateEdge(0, 0));

 // Index larger than number of nodes
 BOOST_CHECK(!graph.ValidateEdge(0, 2));

 // Bad index
 BOOST_CHECK(!graph.ValidateEdge(0, -1));
}

BOOST_AUTO_TEST_CASE( Distance )
{
 // Horizontal
 Graph graph1;
 graph1.AddNode(0.0, 0.0);
 graph1.AddNode(1.0, 0.0);
 BOOST_CHECK_EQUAL(graph1.GetLengthEdge(0, 1), 1.0);

 // Vertical
 Graph graph2;
 graph2.AddNode(5.0, 8.0);
 graph2.AddNode(5.0, 2.0);
 BOOST_CHECK_EQUAL(graph2.GetLengthEdge(0, 1), 6.0);

 // Pythagoras
 Graph graph3;
 graph3.AddNode(2.0, 6.0);
 graph3.AddNode(5.0, 2.0);
 BOOST_CHECK_EQUAL(graph3.GetLengthEdge(0, 1), 5.0);

 // Negative
 Graph graph4;
 graph4.AddNode(2.0, 1);
 graph4.AddNode(-2.0, -2.0);
 BOOST_CHECK_EQUAL(graph4.GetLengthEdge(0, 1), 5.0);

 // Symmetry
 BOOST_CHECK_EQUAL(graph4.GetLengthEdge(0, 1), graph4.GetLengthEdge(1, 0));
}

BOOST_AUTO_TEST_CASE( Visibility )
{
 Graph graph;
 // Horizontal
 graph.AddNode(0.0, 0.0);
 graph.AddNode(1.0, 0.0);
 BOOST_CHECK_EQUAL(graph.GetVisibility(0, 1), graph.GetLengthEdge(0, 1));
}

BOOST_AUTO_TEST_CASE( PathLength )
{
 Graph graph;
 graph.AddNode(0.0, 0.0);
 graph.AddNode(1.0, 0.0);
 graph.AddNode(4.0, 4.0);
 graph.AddNode(4.0, 6.0);
 std::vector<int> path = {0, 1, 2, 3};

 BOOST_CHECK_EQUAL(graph.GetPathLength(path), 8);
}

BOOST_AUTO_TEST_CASE( NearestNeighbour )
{
 Graph graph;
 // Random
 graph.AddNode(0.0, 0.0);
 graph.AddNode(1.0, 0.0);
 BOOST_CHECK_EQUAL(graph.LengthNearestNeighbourPath(), 1);
 graph.AddNode(3.0, 4.0);
 graph.AddNode(3.0, 6.0);
 BOOST_CHECK_EQUAL(graph.LengthNearestNeighbourPath(1), 8);
}

BOOST_AUTO_TEST_SUITE_END()
