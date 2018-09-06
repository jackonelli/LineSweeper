#include "AntSystem.h"
#include <boost/test/auto_unit_test.hpp>
#define BOOST_TEST_MODULE AntSystem test

BOOST_AUTO_TEST_SUITE( AntSystemSuite )
// TODO: Better mock test class
const char * GRAPH_FILE_PATH = "../graph/nodes_test.json";
const unsigned int NUMBER_OF_ANTS = 3;
const float TARGET_PATH_LENGTH = 2;
const float ALPHA = 1;
const float BETA = 1;
const float EVAPORATION = 0.1;
AntSystem antSystem(GRAPH_FILE_PATH, NUMBER_OF_ANTS, TARGET_PATH_LENGTH, ALPHA, BETA, EVAPORATION);

BOOST_AUTO_TEST_CASE( InitPheromoneLevels )
{
  antSystem.InitPheromoneLevels();
  unsigned int numberOfNodes = antSystem.GetNumberOfNodes();
  for(unsigned int i = 0; i< numberOfNodes; i++){
    for(unsigned int j = i+1; j< numberOfNodes; j++){
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(i, j), 1);
      // Symmetry
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(j, i), 1);
    }
  }
}

BOOST_AUTO_TEST_CASE( UpdateDeltaPheromoneLevels )
{
  std::vector<unsigned int> path = {0, 1, 2, 3};
  unsigned int numberOfNodes = antSystem.GetNumberOfNodes();
  std::vector<float> deltaPheromone(numberOfNodes * numberOfNodes , 0);
  antSystem.UpdateDeltaPheromoneLevels(&deltaPheromone, &path);
  BOOST_CHECK_EQUAL(deltaPheromone[1*numberOfNodes + 2], 1.f/3);
  // Asymmetry
  BOOST_CHECK_EQUAL(deltaPheromone[2*numberOfNodes + 1], 0);
  // Unvisited edge
  BOOST_CHECK_EQUAL(deltaPheromone[1*numberOfNodes + 3], 0);
}

BOOST_AUTO_TEST_CASE( UpdatePheromoneLevels )
{
  antSystem.InitPheromoneLevels();
  unsigned int numberOfNodes = antSystem.GetNumberOfNodes();
  std::vector<float> deltaPheromone(numberOfNodes * numberOfNodes , 1);
  antSystem.UpdatePheromoneLevels(&deltaPheromone);

  for(unsigned int i = 0; i< numberOfNodes; i++){
    for(unsigned int j = i+1; j< numberOfNodes; j++){
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(i, j), 2);
      // Symmetry
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(j, i), 2);
    }
  }
  antSystem.InitPheromoneLevels();
  for(unsigned int i = 0; i< numberOfNodes; i++){
    for(unsigned int j = i+1; j< numberOfNodes; j++){
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(i, j), 1);
      // Symmetry
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(j, i), 1);
    }
  }
}

BOOST_AUTO_TEST_CASE( ResetPheromoneLevel )
{
  antSystem.InitPheromoneLevels();
  unsigned int numberOfNodes = antSystem.GetNumberOfNodes();
  for(unsigned int i = 0; i< numberOfNodes; i++){
    for(unsigned int j = i+1; j< numberOfNodes; j++){
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(i, j), 1);
      // Symmetry
      BOOST_CHECK_EQUAL(antSystem.GetPheromoneLevel(j, i), 1);
    }
  }
}

BOOST_AUTO_TEST_CASE( GeneratePath )
{
  antSystem.GeneratePath();
}

BOOST_AUTO_TEST_CASE( Run )
{
  antSystem.ImprovePath(5);
}

BOOST_AUTO_TEST_SUITE_END()
