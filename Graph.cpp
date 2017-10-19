#include "Graph.h"
#include <fstream>
#include <math.h>
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
    while ( nodeFile >> numberOfNodes_ >> x >> y ){
      numberOfNodes_++;
      //std::cout << "Node: "<< numberOfNodes_ << ",\tx = " << x << ",\ty = " << y << std::endl;
      xNode_.push_back(x);
      yNode_.push_back(y);
    }
    nodeFile.close();
  }
  else std::cout << "Could not open node file" << std::endl;

  edgesConnected_.resize(numberOfNodes_ * numberOfNodes_);
  std::ifstream edgeFile(edgeFileName);  
  if(edgeFile.is_open()){
    int iNode, jNode;
    while ( edgeFile >> iNode >> jNode ){
      edgesConnected_[iNode*numberOfNodes_ + jNode] = 1;
    }
    edgeFile.close();
  }
  else std::cout << "Could not open edge file" << std::endl;
}
int Graph::GetNumberOfNodes(){
  return numberOfNodes_;
};
float Graph::GetLengthEdge(const int node_j, const int node_i){
  float deltaX = xNode_[node_j] - xNode_[node_i];
  float deltaY = yNode_[node_j] - yNode_[node_i];
  return sqrtf( deltaX * deltaX + deltaY * deltaY );
};
float Graph::GetVisibility(const int node_j, const int node_i){};

/*void printConnectedEdges(){
  numberOfNodes_ = this.numberOfNodes_;
  edgesConnected_ = this.edgesConnected_;
  for(int i = 0; i < numberOfNodes_; i++){
    for(int j = 0; j < numberOfNodes_; j++){
      std::cout << edgesConnected_[i*numberOfNodes_ + j] << " ";
    }
    std::cout << std::endl;
  }
};*/
