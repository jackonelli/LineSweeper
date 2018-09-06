#include "AntSystem.h"
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
#include <utility>
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

AntSystem::AntSystem(std::string graphFilePath, const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  : graph_(graphFilePath), numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
  graph_.GraphFromFile();
};

AntSystem::AntSystem(const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  :numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
};

AntSystem::~AntSystem(){
};

unsigned int AntSystem::GetNumberOfNodes(){
  return graph_.GetNumberOfNodes();
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

void AntSystem::InitPheromoneLevels(){
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  const float lengthNearestNeighbourPath = graph_.LengthNearestNeighbourPath();
  float initialPheromoneLevel = -1;
  if(lengthNearestNeighbourPath > 0){
    initialPheromoneLevel = numberOfAnts_ / (lengthNearestNeighbourPath);
  } else {
    const std::string s = "Node not in graph";
    std::cout << s << std::endl;
    throw s;
  }
  pheromoneLevel_ = std::vector<float>(numberOfNodes * numberOfNodes, initialPheromoneLevel);
};

void AntSystem::UpdateDeltaPheromoneLevels(std::vector<float> * deltaPheromone, const std::vector<unsigned int> * path){
  const float pathLength = graph_.GetPathLength(path);
  const unsigned int numberOfNodesInPath = path->size();
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  if (pathLength > 0 && numberOfNodesInPath >= numberOfNodes){
    unsigned int currentNode, nextNode;

    for(unsigned int iNode = 0; iNode < numberOfNodesInPath - 1; iNode++ ){
        currentNode = (*path)[iNode] ;
        nextNode = (*path)[iNode+1] ;
        if(graph_.ValidateEdge(currentNode, nextNode)){
          (*deltaPheromone)[currentNode * numberOfNodes + nextNode] += 1 / pathLength;
        } else {
          const std::string s = "Invalid edge";
          std::cout << s << std::endl;
          std::cout << currentNode << ", " << nextNode << std::endl;
          throw s;
        };
    }
  } else {
    const std::string s = "Incorrect path";
    std::cout << s << std::endl;
    throw s;
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
    const std::string s = "Invalid edge";
    std::cout << s << std::endl;
    std::cout << iNode << ", " << jNode << std::endl;
    throw s;
  }
}

std::vector<unsigned int> AntSystem::GeneratePath(){
  unsigned int currentNode, nextNode;
  const unsigned int numberOfNodes = graph_.GetNumberOfNodes();
  std::vector<unsigned int> path;
  path.reserve(numberOfNodes);
  std::unordered_set<unsigned int> unvisitedNodes = ResetUnvisitedNodes();
  const unsigned int startingNode = rand() % numberOfNodes;
  path.push_back(startingNode);
  unvisitedNodes.erase(startingNode);
  currentNode = startingNode;
  for(unsigned int i=0; i < numberOfNodes - 1; i++) {
    nextNode = GetNextNode(currentNode, unvisitedNodes);
    path.push_back(nextNode);
    currentNode = nextNode;
    unvisitedNodes.erase(currentNode);
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
  CalculateTransitionProbability(&transitionProbability, &unvisitedNodes, currentNode);
  return RouletteWheelSelection(&transitionProbability);
}

void AntSystem::CalculateTransitionProbability(std::vector<std::pair<unsigned int, float>> *transitionProbability, const std::unordered_set<unsigned int> *unvisitedNodes, const unsigned int currentNode){
  float totalProbability = 0;
  for(auto node : *unvisitedNodes) { // Sort while creating
    float tmpProbability = GetPheromoneLevel(currentNode, node) * alpha_ * graph_.GetVisibility(currentNode, node) * beta_;
    transitionProbability->push_back(std::make_pair(node,tmpProbability));
    totalProbability += tmpProbability;
  }
  for (auto &probPair : *transitionProbability) probPair.second /= totalProbability;
  std::sort(transitionProbability->begin(), transitionProbability->end(), PairSortDescValue); // Sort descending
}

unsigned int AntSystem::RouletteWheelSelection(const std::vector<std::pair<unsigned int, float>> *transitionProbability){
  const float r = (float) rand() / (float) RAND_MAX; // U(0,1)
  unsigned int index= 0;
  std::pair<unsigned int, float> probPair = (*transitionProbability)[index];
  float cumulativeProbability = probPair.second;
  while(cumulativeProbability < r && index < transitionProbability->size()){
    index++;
    probPair = (*transitionProbability)[index];
    cumulativeProbability += probPair.second;
  }
  return probPair.first;
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
      float pathLength = graph_.GetPathLength(&path);
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

void AntSystem::PrintPath(const std::vector<unsigned int> *path){
  graph_.PrintPath(path);
}

bool AntSystem::PairSortDescValue(const std::pair<unsigned int,float> &a, const std::pair<unsigned int,float> &b)
{
       return (a.second > b.second);
}

void AntSystem::StoreBestPath(){
  graph_.StorePath(&shortestPath_);
}

#ifdef __EMSCRIPTEN__
namespace emscripten{

  EMSCRIPTEN_BINDINGS(antSystemEntry) {
    class_<AntSystem>("AntSystem")
      //.constructor<std::string*, std::string, const unsigned int, const float, const float, const float, const float>()
      .constructor<const unsigned int, const float, const float, const float, const float>()
      .function("InitPheromoneLevels", &AntSystem::InitPheromoneLevels)
      //.property("x", &AntSystem::getX, &AntSystem::setX)
      //.class_function("getStringFromInstance", &AntSystem::getStringFromInstance)
      ;
  }
} // end namespace emscripten
#endif
