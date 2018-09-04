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
    unsigned int GetNumberOfNodes();
    std::vector<unsigned int> GetNodeIds();
    float GetLengthEdge(const unsigned int node1, const unsigned int node2);
    float GetVisibility(const unsigned int node1, const unsigned int node2);
    bool ValidateEdge(const unsigned int node1, const unsigned int node2);
    float LengthNearestNeighbourPath();
    float LengthNearestNeighbourPath(const unsigned int startingNode);
    float GetPathLength(const std::vector<unsigned int> path);
    void PrintConnectedEdges();
    void PrintNodes();
    void PrintPath(const std::vector<unsigned int> path);

  private:
    typedef struct {
      float x;
      float y;
    } Node;
    const char * nodeFileName_, * edgeFileName_;
    unsigned int numberOfNodes_;
    std::unordered_map <unsigned int, Node> nodes_;
    std::vector<bool> edgesConnected_;

};
#endif
