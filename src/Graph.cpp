#include "Graph.h"
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <iostream> // TODO: remove
#include <numeric>

Graph::Graph()
{
  numberOfNodes_ = 0;
}

Graph::Graph(const char * nodeFileName, const char * edgeFileName) : nodeFileName_(nodeFileName), edgeFileName_(edgeFileName)
{
  numberOfNodes_ = 0;
}
Graph::~Graph(){

};
bool Graph::GraphFromFile(){
  std::ifstream nodeFile(nodeFileName_);
  if(nodeFile.is_open()){
    float x,y;
    while ( nodeFile >>  x >> y ){
      AddNode(x, y);
    }
    nodeFile.close();
  }
  else {
    std::cout << "Could not open node file" << std::endl;
    return false;
  }
  edgesConnected_.resize(numberOfNodes_ * numberOfNodes_);
  std::ifstream edgeFile(edgeFileName_);
  if(edgeFile.is_open()){
    int iNode, jNode;
    while ( edgeFile >> iNode >> jNode ){
      edgesConnected_[iNode*numberOfNodes_ + jNode] = 1;
      edgesConnected_[jNode*numberOfNodes_ + iNode] = 1; // EPA make symmetric
    }
    edgeFile.close();
  }
  else {
    std::cout << "Could not open edge file" << std::endl;
    return false;
  }
  return true;
}

void Graph::AddNode(float x, float y){
  xNode_.push_back(x);
  yNode_.push_back(y);
  numberOfNodes_++;
};

int Graph::GetNumberOfNodes(){
  return numberOfNodes_;
};
float Graph::GetLengthEdge(const int jNode, const int iNode){
  if (ValidateEdge(jNode, iNode)){
    float deltaX = xNode_[jNode] - xNode_[iNode];
    float deltaY = yNode_[jNode] - yNode_[iNode];
    return sqrtf( deltaX * deltaX + deltaY * deltaY );
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

float Graph::GetVisibility(const int jNode, const int iNode){
  if (ValidateEdge(jNode, iNode)){
      return 1 / GetLengthEdge(jNode, iNode);
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

bool Graph::ValidateEdge(const int jNode, const int iNode) {
  bool iBounded = iNode < numberOfNodes_ && iNode >= 0;
  bool jBounded = jNode < numberOfNodes_ && jNode >= 0;
  bool differentNodes = jNode != iNode;
  return iBounded && jBounded && differentNodes;
};

float Graph::LengthNearestNeighbourPath(){
  const int startingNode = rand() % numberOfNodes_;
  return LengthNearestNeighbourPath(startingNode);
};

float Graph::LengthNearestNeighbourPath(const int startingNode){
  float dist, minDist;
  float totalDist = 0;
  int index, minIndex;
  int currentNode;
  std::vector<int> unvisitedNodes(numberOfNodes_);
  std::vector<int> path;

  path.push_back(startingNode);
  std::iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
  currentNode = startingNode;
  unvisitedNodes.erase(unvisitedNodes.begin() + startingNode);

  for(int i = 0; i < numberOfNodes_-1; i++){
    minDist = 100;
    index = 0;
    for (auto node : unvisitedNodes) {
      dist = GetLengthEdge(node, currentNode);
      if(dist < minDist){
        minDist = dist;
        minIndex = index;
      }
      index++;
    }
    currentNode = unvisitedNodes[minIndex];
    path.push_back(currentNode);
    unvisitedNodes.erase(unvisitedNodes.begin() + minIndex);
    totalDist += minDist;
  }
  return totalDist;
};

float Graph::GetPathLength(const std::vector<int> path){
  const int n = path.size();
  float pathLength = 0;
  for( int i = 0; i < (n-1); i++){
    pathLength += GetLengthEdge(path[i+1], path[i]);
  }
  return pathLength;
};

void Graph::PrintNodes(){
  for(int i = 0; i < numberOfNodes_; i++){
      std::cout << "Node: "<< i << ",\tx = " << xNode_[i] << ",\ty = " << yNode_[i] << std::endl;
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

void Graph::PrintPath(const std::vector<int> path){
  for(auto node : path) std::cout << node << " --> ";
};
