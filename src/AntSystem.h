#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H
#include <vector>
#include "Graph.h"
class AntSystem
{
  public:
    AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation);

    ~AntSystem();

    void InitPheromoneLevels();

    std::vector<int> GeneratePath(const int ant, const std::vector<float> * pheromoneLevel_);

    int GetNextNode(const int currentNode, const std::vector<int> unvisitedNodes);

    void ComputeDeltaPheromoneLevels(std::vector<float> * deltaPheromoneLevels, const std::vector<int> * path, float pathLength);

    float GetPheromoneLevel(const int node1, const int node2);

    void Run();

  private:
    Graph graph; // How to make graph const?
    const int numberOfAnts_;
    const float targetPathLength_;
    const float alpha_;
    const float beta_;
    const float evaporation_;
    std::vector<float> pheromoneLevel_;
};
#endif
