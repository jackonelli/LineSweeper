#include "Graph.h"
#include <fstream>
#include <iostream> // TODO: remove

using namespace std;
Graph::Graph(const char* filename)
{
  ifstream graphFile(filename);  
  if(graphFile.is_open()){
    float x,y;
    bool traverse;
    while ( graphFile >> x >> y >> traverse){
      cout << x << " " << y << " " << traverse << endl;
    }
    graphFile.close();
  }   
  else cout << "Could not open file" << endl;
}
Graph::~Graph(){

};
int Graph::GetNumberOfNodes(){};
float Graph::GetLengthEdge(int node_j, int node_i){};
float Graph::GetVisibility(int node_j, int node_i){};

