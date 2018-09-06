#ifndef GRAPH_H
#define GRAPH_H
#include "json.hpp"
#include <vector>
#include <string>
#include <unordered_map>
using json = nlohmann::json;

class Graph
{
  public:
    Graph(std::string graphFilePath_);
    Graph();
    ~Graph();
    void GraphFromFile();
    void GraphFromFile(const std::string graphFilePath);
    void AddNodes(json * graphData);
    void AddEdges(json *graphData);
    void AddEdge(const unsigned int source, const unsigned int target);
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
    void PrintPath(const std::vector<unsigned int> *path);

  private:
    typedef struct {
      float x;
      float y;
    } Node;
    std::string graphFilePath_;
    unsigned int numberOfNodes_;
    std::unordered_map <unsigned int, Node> nodes_;
    std::vector<bool> edgesConnected_;

};
#endif
