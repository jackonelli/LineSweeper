#include "AntSystem.h"
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <algorithm>

AntSystem::AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  : graph(nodeFileName, edgeFileName), numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
  //InitPheromoneLevels();
  graph.LengthNearestNeighbourPath();
};

AntSystem::~AntSystem(){
};

void AntSystem::InitPheromoneLevels(){
  int numberOfNodes = graph.GetNumberOfNodes();
  float nearestNeighbourDistance = graph.LengthNearestNeighbourPath();
  pheromoneLevel_ = std::vector<float>(numberOfNodes * numberOfNodes, nearestNeighbourDistance);
};

void AntSystem::ComputeDeltaPheromoneLevels(std::vector<float> * deltaPheromone, const std::vector<int> * path, const float pathLength){
  const int n = path->size();
  int pos;
  for(int i = 0; i < (n - 1); i++ ){
    if (pathLength > 0){
      pos = (*path)[i+1] ;//* graph.GetNumberOfNodes() + (*path)[i];
      std::cout << pos << std::endl;
      //(*deltaPheromone)[pos] += 1 / pathLength;
    } else {
      std::cout << "Incorrect path, pheromones not updated" << std::endl;
    }
  }
}

float AntSystem::GetPheromoneLevel(const int jNode, const int iNode) {
  if (graph.ValidateEdge(jNode, iNode)){
    int numberOfNodes = graph.GetNumberOfNodes();
    if(jNode != iNode) {
      return pheromoneLevel_[iNode*numberOfNodes + jNode];
    } else {
      std::cout << "Same node" << std::endl;
      return 0;
    }
  } else {
    std::cout << "Index out of range" << std::endl;
    return -1;
  }
};

std::vector<int> AntSystem::GeneratePath(const int ant, const std::vector<float> * pheromoneLevel_){
  int currentNode, nextNode;
  const int numberOfNodes = graph.GetNumberOfNodes();
  std::vector<int> path(numberOfNodes);
  std::vector<int> unvisitedNodes(numberOfNodes);
  std::iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
  const int startingNode = rand() % numberOfNodes;
  path.push_back(startingNode);
  unvisitedNodes.erase(unvisitedNodes.begin() + startingNode);
  currentNode = startingNode;
  for(int i=0; i < numberOfNodes - 1; i++) {
    std::cout << "Unvisited nodes" << std::endl;
    for (auto node : unvisitedNodes) std::cout << node << ", ";
    std::cout << std::endl;
    nextNode = GetNextNode(currentNode, unvisitedNodes);
    std::cout << currentNode << " --> " << nextNode << std::endl;
    path.push_back(nextNode);
    unvisitedNodes.erase(unvisitedNodes.begin() + currentNode);
    currentNode = nextNode;
  }
  return path;
};

int AntSystem::GetNextNode(const int currentNode, const std::vector<int> unvisitedNodes) {
  std::vector<float> transitionProbability(unvisitedNodes.size());
  float totalProbability = 0;
  for(auto node : unvisitedNodes) { // Sort while creating
    float tmpProbability = GetPheromoneLevel(currentNode, node) * alpha_ * graph.GetVisibility(currentNode, node) * beta_;
    transitionProbability[node] = tmpProbability;
    std::cout << "Node 1: " << currentNode << " Node 2: " << node << " Prob: " << tmpProbability << std::endl;
    totalProbability += tmpProbability;
  }
  for (auto &prob : transitionProbability) prob /= totalProbability;
  std::sort(transitionProbability.rbegin(), transitionProbability.rend()); // Sort descending

  for (auto prob : transitionProbability) std::cout << prob << ", " << std::endl;

  // Roulette wheel:
  const float r = (float) rand() / (float) RAND_MAX; // U(0,1)
  //std::cout << "Rand: " << r << std::endl;
  int nextNode= 0;
  float cumulated_probability = transitionProbability[nextNode];
  while(cumulated_probability < r && nextNode < unvisitedNodes.size()){
    nextNode++;
    //std::cout << "cum prob: " << cumulated_probability << std::endl;
    cumulated_probability += transitionProbability[nextNode];
  }
  return nextNode;
}

void AntSystem::Run(){
  int iIteration = 0;
  float minPathLength = 1e9;
  std::vector<int> shortestPath;
  std::vector<float> deltaPheromone(pheromoneLevel_.size(), 0);
  while(minPathLength > targetPathLength_){
    iIteration++;
    std::iota(deltaPheromone.begin(), deltaPheromone.end(), 0);

    for(int kAnt = 0; kAnt < numberOfAnts_; kAnt++){
      std::vector<int> path = GeneratePath(kAnt, &pheromoneLevel_);
      std::cout << "after gen path" << std::endl;
      float pathLength = graph.GetPathLength(path);
      ComputeDeltaPheromoneLevels(&deltaPheromone, &path, pathLength);

      if(pathLength < minPathLength){
        minPathLength = pathLength;
        shortestPath = path;
        std::cout << "Iteration " << iIteration << ", path length = " << minPathLength << std::endl;
      }
    }
  }
};
