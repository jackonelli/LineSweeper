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
    int node2, node1;
    while ( edgeFile >> node2 >> node1 ){
      edgesConnected_[node2*numberOfNodes_ + node1] = 1;
      edgesConnected_[node1*numberOfNodes_ + node2] = 1; // EPA make symmetric
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
  /* TODO: Add duplicate node check.
   *
   */
  bool novelNode = true;
  for(auto &kv : nodes_){
    if(x == kv.second.x && y == kv.second.y){
      novelNode = false;
      std::cout << "Node already exists, node: " << kv.first << std::endl;
    }
  }
  bool idInNodes = nodes_.count(numberOfNodes_) > 0;
  if(novelNode && !idInNodes){
    Node node;
    node.x = x;
    node.y = y;
    nodes_[numberOfNodes_] = node;
    numberOfNodes_++;
  }
};

int Graph::GetNumberOfNodes(){
  return numberOfNodes_;
};

float Graph::GetLengthEdge(const int node1_id, const int node2_id){
  if (ValidateEdge(node1_id, node2_id)){
    Node node1 = nodes_[node1_id];
    Node node2 = nodes_[node2_id];
    float deltaX = node1.x - node2.x;
    float deltaY = node1.y - node2.y;
    return sqrtf( deltaX * deltaX + deltaY * deltaY );
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

float Graph::GetVisibility(const int node1, const int node2){
  if (ValidateEdge(node1, node2)){
      return 1 / GetLengthEdge(node1, node2);
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

bool Graph::ValidateEdge(const int node1, const int node2) {
  bool bounded1 = node1 < numberOfNodes_ && node1 >= 0;
  bool bounded2 = node2 < numberOfNodes_ && node2 >= 0;
  bool differentNodes = node1 != node2;
  return bounded1 && bounded2 && differentNodes;
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
  for(auto &kv : nodes_){
      int id = kv.first;
      Node node = kv.second;
      std::cout << "Node: "<< id << ",\tx = " << node.x << ",\ty = " << node.y << std::endl;
  };
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
