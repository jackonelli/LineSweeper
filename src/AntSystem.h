#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H
#include <vector>
#include <unordered_set>
#include "Graph.h"
class AntSystem
{
  public:
    AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation);
    AntSystem(const int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation);
    ~AntSystem();
    int GetNumberOfNodes();
    void InitPheromoneLevels();
    void UpdateDeltaPheromoneLevels(std::vector<float> * deltaPheromoneLevel, const std::vector<int> * path);
    void UpdatePheromoneLevels(std::vector<float> * deltaPheromoneLevel);
    float GetPheromoneLevel(const int node1, const int node2);
    std::vector<int> GeneratePath();
    std::unordered_set<int> ResetUnvisitedNodes();
    int GetNextNode(const int currentNode, const std::unordered_set<int> unvisitedNodes);
    void Run();

  private:
    static bool PairSortDescValue(const std::pair<int,float> &a, const std::pair<int,float> &b);
    Graph graph_;
    const int numberOfAnts_;
    const float targetPathLength_;
    const float alpha_;
    const float beta_;
    const float evaporation_;
    std::vector<float> pheromoneLevel_;
    std::vector<int> shortestPath_;
};
#endif
