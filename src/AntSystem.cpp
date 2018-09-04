#include "AntSystem.h"
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
#include <utility>

AntSystem::AntSystem(const char * nodeFileName, const char * edgeFileName, const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  : graph_(nodeFileName, edgeFileName), numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
  graph_.GraphFromFile();
};

AntSystem::AntSystem(const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  :numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
};

AntSystem::~AntSystem(){
};

void AntSystem::InitPheromoneLevels(){
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  const float lengthNearestNeighbourPath = graph_.LengthNearestNeighbourPath();
  float initialPheromoneLevel = -1;
  if(lengthNearestNeighbourPath > 0){
    initialPheromoneLevel = numberOfAnts_ / (lengthNearestNeighbourPath);
  } else {
    throw "Non-positive path length";
  }
  pheromoneLevel_ = std::vector<float>(numberOfNodes * numberOfNodes, initialPheromoneLevel);
};

void AntSystem::UpdateDeltaPheromoneLevels(std::vector<float> * deltaPheromone, const std::vector<unsigned int> * path){
  const float pathLength = graph_.GetPathLength(*path);
  const unsigned int numberOfNodesInPath = path->size();
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  if (pathLength > 0 && numberOfNodesInPath >= numberOfNodes){
    unsigned int currentNode, nextNode;

    for(unsigned int iNode = 0; iNode < numberOfNodesInPath - 1; iNode++ ){
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

void AntSystem::UpdatePheromoneLevels(const std::vector<float> * deltaPheromone){
  std::transform (pheromoneLevel_.begin(), pheromoneLevel_.end(), (*deltaPheromone).begin(), pheromoneLevel_.begin(), std::plus<float>());
}

float AntSystem::GetPheromoneLevel(const unsigned int jNode, const unsigned int iNode) {
  if (graph_.ValidateEdge(jNode, iNode)){
    unsigned int numberOfNodes = graph_.GetNumberOfNodes();
      return pheromoneLevel_[iNode*numberOfNodes + jNode];
  } else {
    throw "Invalid node";
  }
};

std::vector<unsigned int> AntSystem::GeneratePath(){
  unsigned int currentNode, nextNode;
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  std::vector<unsigned int> path;
  path.reserve(numberOfNodes);
  //std::vector<unsigned int> unvisitedNodes(numberOfNodes);
  //std::iota(unvisitedNodes.begin(), unvisitedNodes.end(), 0);
  std::unordered_set<unsigned int> unvisitedNodes = ResetUnvisitedNodes();
  const unsigned int startingNode = rand() % numberOfNodes;
  path.push_back(startingNode);
  unvisitedNodes.erase(startingNode);
  currentNode = startingNode;
  for(unsigned int i=0; i < numberOfNodes - 1; i++) {
    nextNode = GetNextNode(currentNode, unvisitedNodes);
    path.push_back(nextNode);
    unvisitedNodes.erase(nextNode);
    currentNode = nextNode;
  }
  return path;
};

std::unordered_set<unsigned int> AntSystem::ResetUnvisitedNodes(){
  std::unordered_set<unsigned int> unvisitedNodes;
  std::vector<unsigned int> nodeIds = graph_.GetNodeIds();
  for(auto id : nodeIds){
    unvisitedNodes.insert(id);
  }
  return unvisitedNodes;
}

unsigned int AntSystem::GetNextNode(const unsigned int currentNode, const std::unordered_set<unsigned int> unvisitedNodes){
  std::vector<std::pair<unsigned int, float>> transitionProbability;
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
  unsigned int index= 0;
  float cumulativeProbability = transitionProbability[index].second;
  while(cumulativeProbability < r && index < unvisitedNodes.size()){
    index++;
    cumulativeProbability += transitionProbability[index].second;
  }
  return transitionProbability[index].first;
}

unsigned int AntSystem::GetNumberOfNodes(){
  return graph_.GetNumberOfNodes();
}

void AntSystem::PrintPath(const std::vector<unsigned int> path){
  graph_.PrintPath(path);
}

unsigned int AntSystem::GetTotalIterations(){
  return totalIterations_;
}

float AntSystem::GetMinPathLength(){
  return minPathLength_;
}

std::vector<unsigned int> AntSystem::GetShortestPath(){
  return shortestPath_;
}

void AntSystem::ImprovePath(const unsigned int maxNumberOfIterations){
  unsigned int localIteration = 0;
  const float previousPathLength = minPathLength_;
  std::vector<float> deltaPheromone(pheromoneLevel_.size(), 0);
  while(minPathLength_ >= previousPathLength && localIteration < maxNumberOfIterations){
    localIteration++;
    std::fill(deltaPheromone.begin(), deltaPheromone.end(), 0);
    for(unsigned int kAnt = 0; kAnt < numberOfAnts_; kAnt++){
      std::vector<unsigned int> path = GeneratePath();
      float pathLength = graph_.GetPathLength(path);
      UpdateDeltaPheromoneLevels(&deltaPheromone, &path);
      if(pathLength < minPathLength_){
        minPathLength_ = pathLength;
        shortestPath_ = path;
      }
      UpdatePheromoneLevels(&deltaPheromone);
    }
  }
  totalIterations_ += localIteration;
};

bool AntSystem::PairSortDescValue(const std::pair<unsigned int,float> &a, const std::pair<unsigned int,float> &b)
{
       return (a.second > b.second);
}
