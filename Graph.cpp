#include "Graph.h"
#include <fstream>
#include <iostream> // TODO: remove

using namespace std;
Graph::Graph(const char* filename)
{
  ifstream graphFile(filename);  
  if(graphFile.is_open()){
    while(getline(graphFile, line)){
      std::cout << line << '\n'
    }
    graphFile.close();
  }   
  else std::cout << "Could not open file" << std::endl;
};
~Graph(){};
int Graph::GetNumberOfNodes(){};
int Graph::GetVisibility(int node_j, int node_i){};
