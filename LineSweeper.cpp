#include <iostream>
//#include "Graph.h"
#include "AntSystem.h"
using namespace std;
int main(){
  srand(time(NULL));
  const char * NODE_FILENAME = "graph/nodes.txt";
  const char * EDGE_FILENAME = "graph/edges.txt";
  const int NUMBER_OF_ANTS = 1;
  const float ALPHA = 1.0;
  const float BETA = 1.0;
  AntSystem * antSystem = new AntSystem(NODE_FILENAME, EDGE_FILENAME, NUMBER_OF_ANTS, ALPHA, BETA);
  //float aba = antSystem->LengthNearestNeighbourPath();
  //std::cout << aba << std::endl;
  //float aba = graph->GetLengthEdge(7, 1);
  //std::cout << aba << std::endl;
  //aba = graph->GetLengthEdge(7, 3);
  //std::cout << aba << std::endl;
  delete antSystem;
}
