#ifndef GRAPH_H
#define GRAPH_H

class Graph
{
  public:
    Graph(const char * graphFile);
    ~Graph();
    int GetNumberOfNodes();
    float GetLengthEdge(int node1, int node2);
    float GetVisibility(int node1, int node2);
    bool GraphFromFile(const char * filename);
};
#endif
