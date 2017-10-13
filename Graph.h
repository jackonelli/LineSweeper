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
    bool GraphFromFile(const char * nodeFileName, const char * edgeFileName);
  private:
    std::vector<float> xNode;
    std::vector<float> yNode;
    int numberOfNodes;
    std::vector< bool > edgesConnected;

};
#endif
