#include <iostream>
#include "Graph.h"
using namespace std;
int main(){
  srand(time(NULL));
  const char * nodeFileName = "graph/nodes.txt";
  const char * edgeFileName = "graph/edges.txt";
  Graph * graph = new Graph(nodeFileName, edgeFileName);
  //graph->PrintConnectedEdges();
  float aba = graph->LengthNearestNeighbourPath();
  std::cout << aba << std::endl;
  delete graph;
}
