#include "Graph.h"
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <iostream> // TODO: remove
#include <numeric>

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
float Graph::GetLengthEdge(const int jNode, const int iNode){
  bool iBounded = iNode < numberOfNodes_ && iNode >= 0;
  bool jBounded = jNode < numberOfNodes_ && jNode >= 0;
  if (iBounded && jBounded){
    float deltaX = xNode_[jNode] - xNode_[iNode];
    float deltaY = yNode_[jNode] - yNode_[iNode];
    return sqrtf( deltaX * deltaX + deltaY * deltaY );
  } else {
    std::cout << "Index out of range" << std::endl;
    return -1;
  }
};

float Graph::GetVisibility(const int jNode, const int iNode){
  bool iBounded = iNode < numberOfNodes_ && iNode >= 0;
  bool jBounded = jNode < numberOfNodes_ && jNode >= 0;
  if (iBounded && jBounded){
    return 1/GetLengthEdge(jNode, iNode);
  } else if(jNode == iNode) {
    std::cout << "Same node" << std::endl;
  } else {
    std::cout << "Index out of range" << std::endl;
    return -1;
  }
};

void Graph::PrintConnectedEdges(){
  for(int i = 0; i < numberOfNodes_; i++){
    for(int j = 0; j < numberOfNodes_; j++){
      std::cout << edgesConnected_[i*numberOfNodes_ + j] << " ";
    }
    std::cout << std::endl;
  }
};

float Graph::LengthNearestNeighbourPath(){
  int index, minIndex;
  float dist, minDist, totalDist;
  int startingNode = rand() % numberOfNodes_;
  int currentNode, nextNode;
  std::vector<int> unvisitedNodes(numberOfNodes_);
  std::iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
  currentNode = startingNode;
  unvisitedNodes.erase(unvisitedNodes.begin() + startingNode);
  for(int i = 0; i < numberOfNodes_-1; i++){
      index = 0;
      minDist = 100;
      minIndex = -1;
      for (auto node = unvisitedNodes.begin(); node != unvisitedNodes.end(); ++node, ++index){
        dist = GetLengthEdge(*node, currentNode);
        std::cout << *node << ", ";
        if(dist < minDist){
          minDist = dist;
          minIndex = index;
        }
      }
      unvisitedNodes.erase(unvisitedNodes.begin() + minIndex);
      totalDist += minDist;
      std::cout << "\n";
    }
  return totalDist;
};

