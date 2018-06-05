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

    void ComputeDeltaPheromoneLevels(std::vector<float> * deltaPheromoneLevels, const std::vector<int> * path, float pathLength);

    void Run();

  private:
    Graph graph; // How to make graph const?
    const int numberOfAnts_;
    const float alpha_;
    const float beta_;
    const float targetPathLength_;
    const float evaporation_;
    std::vector<float> pheromoneLevel_;
};
#endif
