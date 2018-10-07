#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <iostream> // TODO: remove
#include <numeric>
#include "Graph.hpp"
#include "tools/fileIO.cpp"
#include "tools/json.hpp"

using json = nlohmann::json;

Graph::Graph(){
  numberOfNodes_ = 0;
}

Graph::Graph(std::string graphFilePath) : graphFilePath_(graphFilePath){
  numberOfNodes_ = 0;
}

Graph::~Graph(){
}

void Graph::GraphFromFile(){
  GraphFromFile(graphFilePath_);
}

void Graph::GraphFromFile(const std::string graphFilePath){
  json graphData = fileIO::ReadJsonFile(graphFilePath.c_str());
  AddNodes(graphData);
  AddEdges(graphData);
}

void Graph::AddNodes(json& graphData){
  if (graphData.find("nodes") != graphData.end()) {
    json nodes = graphData["nodes"];
    for (auto node : nodes){
      if (node.find("x") != node.end() && node.find("y") != node.end()){
        const float x = node["x"];
        const float y = node["y"];
        AddNode(x, y);
      }
    }
  }
}

void Graph::AddNode(float x, float y){
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
    ++numberOfNodes_;
  }
}

void Graph::AddEdges(json& graphData){
  edgesConnected_.resize(numberOfNodes_ * numberOfNodes_);
  if (graphData.find("edges") != graphData.end()) {
    json edges = graphData["edges"];
    for (auto edge : edges){
      if (edge.find("source") != edge.end() && edge.find("target") != edge.end()){
        const unsigned int sourceNode = edge["source"];
        const unsigned int targetNode = edge["target"];
        AddEdge(sourceNode, targetNode);
      }
    }
  }
}

void Graph::AddEdge(const unsigned int source, const unsigned int target){
  if(ValidateEdge(source, target)){
    edgesConnected_[target*numberOfNodes_ + source] = 1;
    edgesConnected_[source*numberOfNodes_ + target] = 1; // EPA make symmetric
  }
}

unsigned int Graph::GetNumberOfNodes() const{
  return numberOfNodes_;
};

std::vector<unsigned int> Graph::GetNodeIds() const{
  std::vector<unsigned int> ids;
  ids.reserve(nodes_.size());
  for(auto &kv : nodes_ ){
    ids.push_back(kv.first);
  }
  return ids;
}

float Graph::GetLengthEdge(const unsigned int sourceId, const unsigned int targetId) const{
  if (ValidateEdge(sourceId, targetId)){
    const Node source = nodes_.find(sourceId)->second;
    const Node target = nodes_.find(targetId)->second;
    float deltaX = source.x - target.x;
    float deltaY = source.y - target.y;
    return sqrtf( deltaX * deltaX + deltaY * deltaY );
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

float Graph::GetVisibility(const unsigned int source, const unsigned int target) const{
  if (ValidateEdge(source, target)){
      return 1 / GetLengthEdge(source, target);
  } else {
    std::cout << "Invalid edge" << std::endl;
    return -1;
  }
};

bool Graph::ValidateEdge(const unsigned int source, const unsigned int target) const{
  bool boundedSource = source < numberOfNodes_;
  bool boundedTarget = target < numberOfNodes_;
  bool differentNodes = source != target;
  return boundedSource && boundedTarget && differentNodes;
};

float Graph::LengthNearestNeighbourPath() const{
  if(numberOfNodes_ == 0){
    const std::string s = "Empty graph";
    std::cout << s << std::endl;
    throw s;
  }
  const unsigned int startingNode = rand() % numberOfNodes_;
  return LengthNearestNeighbourPath(startingNode);
};

float Graph::LengthNearestNeighbourPath(const unsigned int startingNode) const{
  if(nodes_.count(startingNode) == 0){
    const std::string s = "Node not in graph";
    std::cout << s << std::endl;
    throw s;
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

  for(unsigned int i = 0; i < numberOfNodes_-1; ++i){
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

float Graph::GetPathLength(const std::vector<unsigned int>& path) const{
  // TODO: switch to path pointer
  const unsigned int n = path.size();
  float pathLength = 0;
  for( unsigned int i = 0; i < (n-1); ++i){
    pathLength += GetLengthEdge(path[i+1], path[i]);
  }
  return pathLength;
};

void Graph::PrintNodes() const{
  for(auto &kv : nodes_){
      unsigned int id = kv.first;
      Node node = kv.second;
      std::cout << "Node: "<< id << ",\tx = " << node.x << ",\ty = " << node.y << std::endl;
  };
};

void Graph::PrintConnectedEdges() const{
  for(unsigned int i = 0; i < numberOfNodes_; ++i){
    for(unsigned int j = 0; j < numberOfNodes_; ++j){
      std::cout << edgesConnected_[i*numberOfNodes_ + j] << " ";
    }
    std::cout << std::endl;
  }
};

void Graph::PrintPath(const std::vector<unsigned int>& path) const{
  std::cout << "Path" << std::endl;
  for(auto node : path) std::cout << node << " --> ";
  std::cout << std::endl;
};

void Graph::StorePath(const std::vector<unsigned int>& path) const{
  json graphData = fileIO::ReadJsonFile(graphFilePath_);
  json jsonPath(path);
  graphData["storePath"] = jsonPath;
  graphData["lengthStorePath"] = GetPathLength(path);
  fileIO::WriteJsonToFile(&graphData, graphFilePath_);
}
