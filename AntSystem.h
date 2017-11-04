#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H
#include <vector>
#include "Graph.h"
class AntSystem
{
  public:
    AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float alpha, const float beta, const float targetPathLength);

    ~AntSystem();

    void InitPheromoneLevels();

    std::vector<int> GeneratePath(const int ant, const std::vector<float> * pheromoneLevel_);

    void ComputeDeltaPheromoneLevels(std::vector<float> * deltaPheromoneLevels, const std::vector<int> * path, float pathLength);

    void Run();


  private:
    // Make const?
    Graph graph;
    int numberOfAnts_;
    float alpha_;
    float beta_;
    float targetPathLength_;
    float evaporation;
    std::vector<float> pheromoneLevel_;
};
#endif
