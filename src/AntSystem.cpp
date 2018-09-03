#include "AntSystem.h"
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
#include <utility>

AntSystem::AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  : graph_(nodeFileName, edgeFileName), numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
  graph_.GraphFromFile();
};

AntSystem::AntSystem(const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  :numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
};

AntSystem::~AntSystem(){
};

void AntSystem::InitPheromoneLevels(){
  const int numberOfNodes = graph_.GetNumberOfNodes();
  const float lengthNearestNeighbourPath = graph_.LengthNearestNeighbourPath();
  float initialPheromoneLevel = -1;
  if(lengthNearestNeighbourPath > 0){
    initialPheromoneLevel = numberOfAnts_ / (lengthNearestNeighbourPath);
  } else {
    throw "Non-positive path length";
  }
  pheromoneLevel_ = std::vector<float>(numberOfNodes * numberOfNodes, initialPheromoneLevel);
};

void AntSystem::UpdateDeltaPheromoneLevels(std::vector<float> * deltaPheromone, const std::vector<int> * path){
  const float pathLength = graph_.GetPathLength(*path);
  const int numberOfNodesInPath = path->size();
  const int numberOfNodes = graph_.GetNumberOfNodes();
  if (pathLength > 0 && numberOfNodesInPath >= numberOfNodes){
    int currentNode, nextNode;

    for(int iNode = 0; iNode < numberOfNodesInPath - 1; iNode++ ){
        currentNode = (*path)[iNode] ;//* graph_.GetNumberOfNodes() + (*path)[i];
        nextNode = (*path)[iNode+1] ;//* graph_.GetNumberOfNodes() + (*path)[i];
        if(graph_.ValidateEdge(currentNode, nextNode)){
          (*deltaPheromone)[currentNode * numberOfNodes + nextNode] += 1 / pathLength;
        } else {
          throw "Invalid node";
        };
    }
  } else {
    throw "Incorrect path";
  }
}

void AntSystem::UpdatePheromoneLevels(std::vector<float> * deltaPheromone){
  std::transform (pheromoneLevel_.begin(), pheromoneLevel_.end(), (*deltaPheromone).begin(), pheromoneLevel_.begin(), std::plus<float>());
}

float AntSystem::GetPheromoneLevel(const int jNode, const int iNode) {
  if (graph_.ValidateEdge(jNode, iNode)){
    int numberOfNodes = graph_.GetNumberOfNodes();
      return pheromoneLevel_[iNode*numberOfNodes + jNode];
  } else {
    throw "Invalid node";
  }
};

std::vector<int> AntSystem::GeneratePath(){
  int currentNode, nextNode;
  const int numberOfNodes = graph_.GetNumberOfNodes();
  std::vector<int> path;
  path.reserve(numberOfNodes);
  //std::vector<int> unvisitedNodes(numberOfNodes);
  //std::iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
  std::unordered_set<int> unvisitedNodes = ResetUnvisitedNodes();
  const int startingNode = rand() % numberOfNodes;
  path.push_back(startingNode);
  unvisitedNodes.erase(startingNode);
  currentNode = startingNode;
  for(int i=0; i < numberOfNodes - 1; i++) {
    nextNode = GetNextNode(currentNode, unvisitedNodes);
    path.push_back(nextNode);
    unvisitedNodes.erase(nextNode);
    currentNode = nextNode;
  }
  return path;
};

std::unordered_set<int> AntSystem::ResetUnvisitedNodes(){
  std::unordered_set<int> unvisitedNodes;
  std::vector<int> nodeIds = graph_.GetNodeIds();
  for(auto id : nodeIds){
    unvisitedNodes.insert(id);
  }
  return unvisitedNodes;
}

int AntSystem::GetNextNode(const int currentNode, const std::unordered_set<int> unvisitedNodes){
  std::vector<std::pair<int, float>> transitionProbability;
  transitionProbability.reserve(unvisitedNodes.size());
  float totalProbability = 0;
  for(auto node : unvisitedNodes) { // Sort while creating
    float tmpProbability = GetPheromoneLevel(currentNode, node) * alpha_ * graph_.GetVisibility(currentNode, node) * beta_;
    transitionProbability.push_back(std::make_pair(node,tmpProbability));
    totalProbability += tmpProbability;
  }
  for (auto &probPair : transitionProbability) probPair.second /= totalProbability;
  std::sort(transitionProbability.begin(), transitionProbability.end(), PairSortDescValue); // Sort descending

  // Roulette wheel:
  const float r = (float) rand() / (float) RAND_MAX; // U(0,1)
  int index= 0;
  float cumulated_probability = transitionProbability[index].second;
  while(cumulated_probability < r && index < unvisitedNodes.size()){
    index++;
    //std::cout << "cum prob: " << cumulated_probability << std::endl;
    cumulated_probability += transitionProbability[index].second;
  }
  return transitionProbability[index].first;
}

int AntSystem::GetNumberOfNodes(){
  return graph_.GetNumberOfNodes();
}
void AntSystem::Run(){
  int iIteration = 0;
  float minPathLength = 1e9;
  std::vector<float> deltaPheromone(pheromoneLevel_.size(), 0);
  while(minPathLength > targetPathLength_ && iIteration < 10000){ iIteration++;
    std::fill(deltaPheromone.begin(), deltaPheromone.end(), 0);
    for(int kAnt = 0; kAnt < numberOfAnts_; kAnt++){
      std::vector<int> path = GeneratePath();
      float pathLength = graph_.GetPathLength(path);
      UpdateDeltaPheromoneLevels(&deltaPheromone, &path);
      if(pathLength < minPathLength){
        minPathLength = pathLength;
        shortestPath_ = path;
      }
      UpdatePheromoneLevels(&deltaPheromone);
    }
    std::cout << "Iteration " << iIteration << ", path length = " << minPathLength << std::endl;
  }
  graph_.PrintPath(shortestPath_);
};

bool AntSystem::PairSortDescValue(const std::pair<int,float> &a, const std::pair<int,float> &b)
{
       return (a.second > b.second);
}
