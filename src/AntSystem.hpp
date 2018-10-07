#ifndef ANTSYSTEM_H
#define ANTSYSTEM_H
#include <vector>
#include <unordered_set>
#include "Graph.hpp"
class AntSystem
{
  public:
    AntSystem(std::string graphFilePath, const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation);
    AntSystem(const unsigned int numberOfAnts, const float targetPathLength, const float alpha, const float beta, const float evaporation);
    ~AntSystem();
    unsigned int GetNumberOfNodes() const;
    unsigned int GetTotalIterations() const;
    float GetMinPathLength() const;
    std::vector<unsigned int> GetShortestPath() const;
    void InitPheromoneLevels();
    void UpdateDeltaPheromoneLevels(std::vector<float>& deltaPheromoneLevel, const std::vector<unsigned int>& path) const;
    void UpdatePheromoneLevels(const std::vector<float>& deltaPheromoneLevel);
    float ScaleVector(float element);
    float GetPheromoneLevel(const unsigned int node1, const unsigned int node2) const;
    std::vector<unsigned int> GeneratePath() const;
    std::unordered_set<unsigned int> ResetUnvisitedNodes() const;
    unsigned int GetNextNode(const unsigned int currentNode, const std::unordered_set<unsigned int>& unvisitedNodes) const;
    void CalculateTransitionProbability(std::vector<std::pair<unsigned int, float>>& transitionProbability, const std::unordered_set<unsigned int>& unvisitedNodes, const unsigned int currentNode) const;
    unsigned int RouletteWheelSelection(const std::vector<std::pair<unsigned int, float>>& transitionProbability) const;
    void ImprovePath(const unsigned int maxNumberOfIterations);
    void PrintPath(const std::vector<unsigned int>& path) const;
    void StoreBestPath() const;

  private:
    static bool PairSortDescValue(const std::pair<unsigned int,float>& a, const std::pair<unsigned int,float>& b);
    Graph graph_;
    const unsigned int numberOfAnts_;
    const float targetPathLength_;
    const float alpha_;
    const float beta_;
    const float evaporation_;
    std::vector<float> pheromoneLevel_;
    std::vector<unsigned int> shortestPath_;
    unsigned int totalIterations_ = 0;
    float minPathLength_ = 1e9;
};
#endif
