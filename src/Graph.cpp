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
    unsigned int node2, node1;
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
  bool novelCoord = true;
  for(auto &kv : nodes_){
    if(x == kv.second.x && y == kv.second.y){
      novelCoord = false;
      std::cout << "Node already exists, node: " << kv.first << std::endl;
    }
  }
  bool nodeExist = nodes_.count(numberOfNodes_) > 0;
  if(novelCoord && !nodeExist){
    Node node = {x, y};
    nodes_[numberOfNodes_] = node;
    numberOfNodes_++;
  }
};

unsigned int Graph::GetNumberOfNodes(){
  return numberOfNodes_;
};

std::vector<unsigned int> Graph::GetNodeIds(){
  std::vector<unsigned int> ids;
  ids.reserve(nodes_.size());
  for(auto &kv : nodes_ ){
    ids.push_back(kv.first);
  }
  return ids;
}

float Graph::GetLengthEdge(const unsigned int node1_id, const unsigned int node2_id){
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

float Graph::GetVisibility(const unsigned int node1, const unsigned int node2){
  if (ValidateEdge(node1, node2)){
      return 1 / GetLengthEdge(node1, node2);
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

bool Graph::ValidateEdge(const unsigned int node1, const unsigned int node2) {
  bool bounded1 = node1 < numberOfNodes_;
  bool bounded2 = node2 < numberOfNodes_;
  bool differentNodes = node1 != node2;
  return bounded1 && bounded2 && differentNodes;
};

float Graph::LengthNearestNeighbourPath(){
  if(numberOfNodes_ == 0){
    throw "Empty graph";
  }
  const unsigned int startingNode = rand() % numberOfNodes_;
  return LengthNearestNeighbourPath(startingNode);
};

float Graph::LengthNearestNeighbourPath(const unsigned int startingNode){
  if(nodes_.count(startingNode) == 0){
    throw "Node not in graph";
  }
  float dist, minDist;
  float totalDist = 0;
  unsigned int minId;
  unsigned int currentNode;

  std::vector<unsigned int> path;
  path.push_back(startingNode);
  currentNode = startingNode;
  std::unordered_map<unsigned int, Node> unvisitedNodes(nodes_);
  unvisitedNodes.erase(startingNode);

  for(unsigned int i = 0; i < numberOfNodes_-1; i++){
    minDist = 100;
    for (auto node : unvisitedNodes) {
      dist = GetLengthEdge(node.first, currentNode);
      if(dist < minDist){
        minDist = dist;
        minId = node.first;
      }
    }
    currentNode = minId;
    path.push_back(currentNode);
    unvisitedNodes.erase(minId);
    totalDist += minDist;
  }
  return totalDist;
};

float Graph::GetPathLength(const std::vector<unsigned int> path){
  const unsigned int n = path.size();
  float pathLength = 0;
  for( unsigned int i = 0; i < (n-1); i++){
    pathLength += GetLengthEdge(path[i+1], path[i]);
  }
  return pathLength;
};

void Graph::PrintNodes(){
  for(auto &kv : nodes_){
      unsigned int id = kv.first;
      Node node = kv.second;
      std::cout << "Node: "<< id << ",\tx = " << node.x << ",\ty = " << node.y << std::endl;
  };
};

void Graph::PrintConnectedEdges(){
  for(unsigned int i = 0; i < numberOfNodes_; i++){
    for(unsigned int j = 0; j < numberOfNodes_; j++){
      std::cout << edgesConnected_[i*numberOfNodes_ + j] << " ";
    }
    std::cout << std::endl;
  }
};

void Graph::PrintPath(const std::vector<unsigned int> *path){
  std::cout << "Path" << std::endl;
  for(auto node : *path) std::cout << node << " --> ";
  std::cout << std::endl;
};
