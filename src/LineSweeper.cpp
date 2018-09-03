#include <cstdlib>
#include <ctime>
#include <iostream>
#include "AntSystem.h"
using namespace std;
int main(){
  srand(time(NULL));
  const char * NODE_FILENAME = "graph/nodes.txt";
  const char * EDGE_FILENAME = "graph/edges.txt";
  const int NUMBER_OF_ANTS = 1;
  const float TARGET_PATH_LENGTH = 10;
  const float ALPHA = 1;
  const float BETA = 1;
  const float EVAPORATION = 0.1;
  AntSystem * antSystem = new AntSystem(NODE_FILENAME, EDGE_FILENAME, NUMBER_OF_ANTS, TARGET_PATH_LENGTH, ALPHA, BETA, EVAPORATION);
  antSystem->Run();
  //float aba = antSystem->LengthNearestNeighbourPath();
  //std::cout << aba << std::endl;
  //float aba = graph->GetLengthEdge(7, 1);
  //std::cout << aba << std::endl;
  //aba = graph->GetLengthEdge(7, 3);
  //std::cout << aba << std::endl;
  delete antSystem;
}
