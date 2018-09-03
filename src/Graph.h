#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <unordered_map>

class Graph
{
  public:
    Graph(const char * nodeFileName, const char * edgeFileName);
    Graph();
    ~Graph();
    bool GraphFromFile();
    void AddNode(float x, float y);
    int GetNumberOfNodes();
    std::vector<int> GetNodeIds();
    float GetLengthEdge(const int node1, const int node2);
    float GetVisibility(const int node1, const int node2);
    bool ValidateEdge(const int node1, const int node2);
    float LengthNearestNeighbourPath();
    float LengthNearestNeighbourPath(const int startingNode);
    float GetPathLength(const std::vector<int> path);
    void PrintConnectedEdges();
    void PrintNodes();
    void PrintPath(const std::vector<int> path);

  private:
    typedef struct {
      float x;
      float y;
    } Node;
    const char * nodeFileName_, * edgeFileName_;
    int numberOfNodes_;
    std::unordered_map <int, Node> nodes_;
    std::vector<bool> edgesConnected_;

};
#endif
