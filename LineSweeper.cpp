#include <iostream>
#include "Graph.h"
using namespace std;
int main(){
  const char * nodeFileName = "graph/nodes.txt";
  const char * edgeFileName = "graph/edges.txt";
  Graph * graph = new Graph(nodeFileName, edgeFileName);
  //graph->printConnectedEdges();
  int aba = graph->GetNumberOfNodes();
  std::cout << aba << std::endl;
  delete graph;
}
