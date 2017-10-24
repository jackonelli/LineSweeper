#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H
#include <vector>
#include "Graph.h"
class AntSystem
{
  public:
    AntSystem(const char * nodeFileName, const char * edgeFileName, const int numberOfAnts, const float alpha, const float beta);
    ~AntSystem();
  private:
    Graph graph;
    int numberOfAnts_;
    float alpha_;
    float beta_;
    std::vector<float> pheromoneLevel_;
    /*std::vector<float> xNode_;
    std::vector<float> yNode_;
    int numberOfNodes_;
    std::vector< bool > edgesConnected_;*/

};
#endif
