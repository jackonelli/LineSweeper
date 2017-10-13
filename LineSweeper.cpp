#include <iostream>
#include "Graph.h"
using namespace std;
int main(){
  const char * nodeFileName = "nodes.txt";
  const char * edgeFileName = "edges.txt";
  Graph * graph = new Graph(nodeFileName, edgeFileName);
  delete graph;
}
