#ifndef _GRAPH_H
#define _GRAPH_H

#include "Node.h"
#include <vector>
#include <utility>
#include <tuple>

class Graph
{
private:
    std::unordered_map<std::string, Node *> nodeMap;
    bool directed = true;

public:
    Graph();
    Graph(bool directed);
    Graph(const Graph &other);
    ~Graph();

    bool addNode(double data, std::string name);
    bool addNode(std::string name); // Default data value '1'
    void addNodes(std::vector<std::string> nodes);
    void addNodes(std::vector<std::pair<double, std::string>> nodes);
    bool addEdge(std::string fromNode, std::string toNode, double weight);
    bool addEdge(std::string fromNode, std::string toNode); // Default weight '1'
    bool addEdge(std::tuple<std::string, std::string, double> edge);
    bool deleteNode(std::string targetNode);
    bool deleteEdge(std::string fromNode, std::string toNode, double weight);
    bool deleteEdge(std::string fromNode, std::string toNode); // Default weight '1'

    //Undirected Graph Specific Functions
    bool connected();

    //Neighbor Functions
    std::vector<std::string> neighborsNames(std::string name);
    bool deleteNeighbors(std::string name);

    //Explore Functions
    std::unordered_set<std::string> explore(std::string sourceNode); //Returns a set of Nodes reachable from the source Node
    void exploreHelper(std::unordered_set<std::string> &visited, std::string name);
    std::vector<std::string> reachableNames(std::string sourceNode); //Returns a list of Nodes that are reachable from the target
    bool pathCheck(std::string fromNode, std::string toNode);

    //Core Graph Functions
    std::vector<std::string> BFS(std::string sourceNode, std::string targetNode); //Returns the shortest path from source to target
    std::vector<std::string> DFS(std::string sourceNode, std::string targetNode); //Returns the shortest path from source to target
    void DFShelper(std::string sourceNode, std::string targetNode, std::unordered_map<std::string, std::string> &prevMap);

    //About Graph Functions
    std::string getInfo(); //Returns a list of all Nodes along with their Edges.
    std::vector< std::tuple<std::string, std::string, double> > getEdges() const; //Returns a vector of Edges, where Edges are represented with a 3-tuple (nodeA,nodeB,weight)
    int numNodes(); //Returns the number of Nodes
    int numEdges();
    bool nodeExists(std::string node); //Is the Node in the Graph?
};

#endif 