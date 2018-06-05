#include "AntSystem.h"
#include <iostream>
#include <numeric>
AntSystem::AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation)
  : graph(nodeFileName, edgeFileName), numberOfAnts_(numberOfAnts), targetPathLength_(targetPathLength), alpha_(alpha), beta_(beta), evaporation_(evaporation) {
  InitPheromoneLevels();
};

AntSystem::~AntSystem(){
};

void AntSystem::InitPheromoneLevels(){
  int numberOfNodes = graph.GetNumberOfNodes();
  float D_NN = graph.LengthNearestNeighbourPath();
  pheromoneLevel_ = std::vector<float>(numberOfNodes * numberOfNodes, D_NN);
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
      std::cout << "Incorrec path, pheromones not updated" << std::endl;
    }
  }
}

std::vector<int> AntSystem::GeneratePath(const int ant, const std::vector<float> * pheromoneLevel_){
  int numberOfNodes = graph.GetNumberOfNodes();
  int startingNode = rand() % numberOfNodes;
  std::vector<int> path;


  return path;
};

void AntSystem::Run(){
  int iIteration = 0;
  float minPathLength = 1e9;
  std::vector<int> shortestPath;
  std::vector<float> deltaPheromone(pheromoneLevel_.size(), 0);
  while(minPathLength > targetPathLength_){
    iIteration++;
    std::iota(deltaPheromone.begin(), deltaPheromone.end(),0);

    for(int kAnt = 0; kAnt < numberOfAnts_; kAnt++){
      std::vector<int> path = GeneratePath(kAnt, &pheromoneLevel_);
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
