#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

class Graph
{
  public:
    Graph(const char * nodeFileName, const char * edgeFileName);
    ~Graph();
    int GetNumberOfNodes();
    float GetLengthEdge(const int node1, const int node2);
    float GetVisibility(const int node1, const int node2);
    bool ValidateEdge(const int node1, const int node2);
    bool GraphFromFile(const char * nodeFileName, const char * edgeFileName);
    void PrintConnectedEdges();
    float LengthNearestNeighbourPath();
    float GetPathLength(const std::vector<int> path);

  private:
    std::vector<float> xNode_;
    std::vector<float> yNode_;
    int numberOfNodes_;
    std::vector< bool > edgesConnected_;
};
#endif
