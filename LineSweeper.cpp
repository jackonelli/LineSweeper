#include <iostream>
#include "Graph.h"
using namespace std;
int main(){
  const char * filename = "dummy.txt";
  Graph * graph = new Graph(filename);
  delete graph;
}
