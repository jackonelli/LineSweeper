#include "Graph.h"
#include <fstream>
#include <iostream> // TODO: remove

Graph::Graph(const char * nodeFileName, const char * edgeFileName)
{
  GraphFromFile(nodeFileName, edgeFileName);
}
Graph::~Graph(){

};
bool Graph::GraphFromFile(const char * nodeFileName, const char * edgeFileName){
  std::ifstream nodeFile(nodeFileName);  
  if(nodeFile.is_open()){
    float x,y;
    while ( nodeFile >> numberOfNodes >> x >> y ){
      numberOfNodes++;
      std::cout << "Node: "<< numberOfNodes << ",\tx = " << x << ",\ty = " << y << std::endl;
      xNode.push_back(x);
      yNode.push_back(y);
    }
    nodeFile.close();
  }   
  else std::cout << "Could not open node file" << std::endl;
  
  edgesConnected.resize(numberOfNodes * numberOfNodes);
  std::ifstream edgeFile(edgeFileName);  
  if(edgeFile.is_open()){
    int iNode, jNode;
    while ( edgeFile >> iNode >> jNode ){
      edgesConnected[iNode*numberOfNodes + jNode] = 1;
    }
    edgeFile.close();
  }   
  else std::cout << "Could not open edge file" << std::endl;

  for(int i = 0; i < numberOfNodes; i++){
    for(int j = 0; j < numberOfNodes; j++){
      std::cout << edgesConnected[i*numberOfNodes + j] << " ";
    }
    std::cout << std::endl;
  }
}
int Graph::GetNumberOfNodes(){};
float Graph::GetLengthEdge(const int node_j, const int node_i){};
float Graph::GetVisibility(const int node_j, const int node_i){};

