#include <cstdlib>
#include <ctime>
#include <iostream>
#include "AntSystem.h"
using namespace std;
int main(){
  srand(time(NULL)); // Does this even work?
  std::string GRAPH_FILE_PATH = "../web/graph/graph.json";
  const int NUMBER_OF_ANTS = 5;
  const float TARGET_PATH_LENGTH = 10;
  const float ALPHA = 1;
  const float BETA = 1;
  const float EVAPORATION = 0.1;
  AntSystem antSystem(GRAPH_FILE_PATH, NUMBER_OF_ANTS, TARGET_PATH_LENGTH, ALPHA, BETA, EVAPORATION);
  antSystem.InitPheromoneLevels();
  std::vector<unsigned int> shortestPath;
  shortestPath.reserve(antSystem.GetNumberOfNodes());
  while(antSystem.GetTotalIterations() < 20){
    std::cout << "Iteration " << antSystem.GetTotalIterations() << ", path length = " << antSystem.GetMinPathLength() << std::endl;
    shortestPath = antSystem.GetShortestPath();
    antSystem.PrintPath(&shortestPath);
    antSystem.ImprovePath(10);
  }
  antSystem.StoreBestPath();
}
