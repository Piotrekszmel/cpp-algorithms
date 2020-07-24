#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <queue>

Graph::Graph() {}

Graph::Graph(bool directed)
{
    this->directed = directed;
}

Graph::Graph(const Graph& original)
{
    directed = original.directed;

    //Add all nodes in original to new Graph
    for (auto iter : original.nodeMap)
    {
        double data = iter.second->getData();
        std::string name = iter.first;

        Node *newNode = new Node(data, name);
        nodeMap.emplace(name, newNode);
    }

    //Add all edges in original to new Graph
    std::vector<std::tuple<std::string, std::string, double>> edgeVec = original.getEdges();
    for (auto edge : edgeVec)
    {
        std::string nodeA = std::get<0>(edge);
        std::string nodeB = std::get<1>(edge);
        double weight = std::get<2>(edge);

        this->addEdge(nodeA, nodeB, weight);
    }
}

Graph::~Graph()
{
    for (auto iter : nodeMap)
    {
        delete iter.second;
    }
}

bool Graph::addNode(double data, std::string name)
{
    if (nodeMap.find(name) != nodeMap.end())
    {
        return false;
    }
    Node *newNode = new Node(data, name);
    nodeMap.emplace(name, newNode);

    return true;
}

bool Graph::addNode(std::string name)
{
    return addNode(1, name);
}

void Graph::addNodes(std::vector<std::pair<double, std::string>> nodes)
{
    for (auto nodePair : nodes)
    {
        addNode(nodePair.first, nodePair.second);
    }
}

bool Graph::addEdge(std::string fromNode, std::string toNode, double weight)
{
    //If one of the nodes don't exist, return false
    if (nodeMap.find(fromNode) == nodeMap.end()) { return false; }
    if (nodeMap.find(toNode) == nodeMap.end()) { return false; }

    nodeMap[fromNode]->addNeighbor(toNode, weight);
    nodeMap[toNode]->getSetRef().insert(fromNode);

    //If the Graph is undirected, also add the "Inverse-Edge"
    if (!directed)
    {
        nodeMap[toNode]->addNeighbor(fromNode, weight);
        nodeMap[fromNode]->getSetRef().insert(toNode);
    }

    return true;
}

bool Graph::addEdge(std::string fromNode, std::string toNode)
{
    return addEdge(fromNode, toNode, 1);
}

///Add Edge using a 3-tuple (nodeA,nodeB,weight)
bool Graph::addEdge(std::tuple<std::string, std::string, double> edge)
{
    return addEdge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
}

bool Graph::deleteNode(std::string targetNode)
{
    if (nodeMap.find(targetNode) == nodeMap.end()) { return false; }

    //For each Node N in getSetRef(), remove targetNode from N's getMapPtr()
    //getSetRef() will have all Nodes that have an edge to targetNode
    std::unordered_set<std::string>& setReference = (nodeMap[targetNode]->getSetRef());
    for (auto iter : setReference)
    {
        (nodeMap[iter]->getMapPtr())->erase(targetNode);
    }

    //Remove targetNode from it's neighbors "getSetRef()"
    for (auto iter : *(nodeMap[targetNode]->getMapPtr()))
    {
        nodeMap[iter.first]->getSetRef().erase(targetNode);
    }

    //Deallocate Node, remove it from nodeMap
    delete nodeMap[targetNode];
    nodeMap.erase(targetNode);
    return true;
}


bool Graph::deleteEdge(std::string fromNode, std::string toNode, double weight)
{
    //If one of the nodes don't exist or no such edge exists, return false
    if (nodeMap.find(fromNode) == nodeMap.end()) { return false; }
    if (nodeMap.find(toNode) == nodeMap.end()) { return false; }
    std::unordered_map<std::string, std::multiset<double>>& neighborMapRef = *(nodeMap[fromNode]->getMapPtr());
    if (neighborMapRef.find(toNode) == neighborMapRef.end()) { return false; }

    //Delete weight from multiset
    std::multiset<double>& set = neighborMapRef[toNode];
    set.erase(weight);

    //If that was the last edge from fromNode to toNode, delete that (key,value) pair from getMapPtr()
    if (set.empty())
    {
        neighborMapRef.erase(toNode);
    }

    //If the Graph is undirected, also delete the "Inverse-Edge"
    if (!directed)
    {
        std::unordered_map<std::string, std::multiset<double>>& neigborMapRef2 = *(nodeMap[toNode]->getMapPtr());

        std::multiset<double>& set2 = neigborMapRef2[fromNode];
        set2.erase(weight);

        if (set2.empty())
        {
            neigborMapRef2.erase(fromNode);
        }
    }

    return true;
}

bool Graph::deleteEdge(std::string fromNode, std::string toNode)
{
    return deleteEdge(fromNode, toNode, 1);
}

///connected: Returns true if the Graph is connected, for undirected Graphs.
bool Graph::connected()
{
    if (nodeMap.empty()) { return true; } //An empty Graph is trivially connected

    auto it = nodeMap.begin();
    std::unordered_set<std::string> tempSet = explore(it->first);

    return (tempSet.size() == nodeMap.size());
}

std::vector<std::string> Graph::neighborsNames(std::string sourceNode)
{
    std::vector<std::string> names;

    std::unordered_map<std::string, std::multiset<double>> *neighborMapPtr = nodeMap[sourceNode]->getMapPtr();
    for (auto it : *neighborMapPtr)
    {
        names.push_back(it.first);
    }
}

///deleteNeighbors: Removes all neighbors of sourceNode along with all the edges associated with the neighbors.
bool Graph::deleteNeighbors(std::string sourceNode)
{
    if (nodeMap.find(sourceNode) == nodeMap.end()) { return false; }

    std::vector<std::string> neighbors = neighborsNames(sourceNode);
    for (auto neighbor : neighbors)
    {
        deleteNode(neighbor);
    }
    return true;
}

std::unordered_set<std::string> Graph::explore(std::string sourceNode)
{
    std::unordered_set<std::string> reachable;
    exploreHelper(reachable, sourceNode);
    return reachable;
}

void Graph::exploreHelper(std::unordered_set<std::string> &visited, std::string name)
{
    visited.insert(name);
    std::vector<std::string> neighbors = neighborsNames(name);

    for (auto neighbor : neighbors)
    {
        if (visited.find(neighbor) == visited.end())
        {
            exploreHelper(visited, neighbor);
        }
    }
}

///reachableNames: Returns a list of Nodes reachable from a given sourceNode
std::vector<std::string> Graph::reachableNames(std::string sourceNode)
{
    std::vector<std::string> names;
    std::unordered_set<std::string> reachable = explore(sourceNode);
    for (std::string name : reachable)
    {
        names.push_back(name);
    }

    return names;
}

///pathCheck: Returns true if there is a (directed) path from fromNode to toNode.
bool Graph::pathCheck(std::string sourceNode, std::string targetNode)
{
    std::unordered_set<std::string> reachable = explore(sourceNode);
    return (reachable.find(targetNode) != reachable.end());
}

std::vector<std::string> Graph::BFS(std::string sourceNode, std::string targetNode)
{
    //If either Node DNE, return an empty vector
    std::vector<std::string> pathVec;
    if (nodeMap.find(sourceNode) == nodeMap.end()) { return pathVec; }
    if (nodeMap.find(targetNode) == nodeMap.end()) { return pathVec; }

    std::unordered_map<std::string, std::string> prevMap;
    prevMap.emplace(sourceNode, "");

    //BFS
    std::queue<std::string> Q;
    Q.push(sourceNode);

    while(!Q.empty())
    {
        std::string currNode = Q.front();
        Q.pop();
        std::vector<std::string> neighborsCurr = neighborsNames(currNode);
        for (std::string N : neighborsCurr)
        {
            if (prevMap.find(N) == prevMap.end())
            {
                Q.push(N);
                prevMap.emplace(N, currNode);
            }
        }
    }

    if (prevMap.find(targetNode) == prevMap.end()) { return pathVec; }

    //Use prevMap to get the path from Target back to Source
    std::string curr = targetNode;
    pathVec.push_back(curr);
    while (true)
    {
        curr = prevMap[curr];
        if (curr == "") { break; }
        pathVec.push_back(curr);
    }

    //Reverse pathVec so the Node's are in order from Source to Target
    std::reverse(pathVec.begin(), pathVec.end());

    return pathVec;
}

///DFS: Returns the shortest unweighted path from sourceNode to targetNode
std::vector<std::string> Graph::DFS(std::string sourceNode, std::string targetNode)
{
    std::vector<std::string> pathVec;
    if (nodeMap.find(sourceNode) == nodeMap.end()) { return pathVec; }
    if (nodeMap.find(targetNode) == nodeMap.end()) { return pathVec; }

    std::unordered_map<std::string, std::string> prevMap;
    prevMap.emplace(sourceNode, "");

    DFShelper(sourceNode, targetNode, prevMap);

    if (prevMap.find(targetNode) == prevMap.end()) { return pathVec; }

    std::string curr = targetNode;
    pathVec.push_back(curr);
    while (true) {
        curr = prevMap[curr];
        if (curr == "") { break; }
        pathVec.push_back(curr);
    }

    //Reverse pathVec so the Node's are in order from Source to Target
    std::reverse(pathVec.begin(), pathVec.end());

    return pathVec;
}

void Graph::DFShelper(std::string currentNode, std::string targetNode, std::unordered_map<std::string, std::string> &prevMap)
{
    if (currentNode == targetNode) { return; }

    std::vector<std::string> neighbors = neighborsNames(currentNode);
    for (std::string neighbor : neighbors)
    {
        //If this neighbor has not been visited, add it to the prevMap and recurse on it
        if (prevMap.find(neighbor) == prevMap.end())
        {
            prevMap.emplace(neighbor, currentNode);
            DFShelper(neighbor, currentNode, prevMap);
        }
    }
}

std::string Graph::getInfo()
{
     std::stringstream ss;
     ss << std::fixed;
     ss << "\n\nGraph Info: " << '\n';

     for (auto iterA: nodeMap)
     {
         ss << "[" << iterA.first << ": " << iterA.second->getData() << "] ";
         for (auto iterB : *(iterA.second->getMapPtr()))
         {
             ss << "(" << iterB.first << "): ";
             for (auto weight : iterB.second)
             {
                 ss << weight << ", ";
             }
         }
         ss << "\n\n";
     }
     return ss.str();
}

///getEdges: Returns an unsorted vector of edges, where edges are represented with 3-tuples (nodeA, nodeB, weight)
std::vector<std::tuple<std::string, std::string, double>> Graph::getEdges() const
{
    std::vector<std::tuple<std::string, std::string, double>> edgeVec;

    //For all Nodes K in nodeMap
    for (auto iter : nodeMap)
    {
        auto K = iter.second; // K is a Node*
        for (auto iter2 : *(K->getMapPtr()))
        {
            auto tempSet = iter2.second; // tempSet is an multiset
            for (double i: tempSet)
            {
                std::string nodeA = iter.first;
                std::string nodeB = iter2.first;
                std::tuple<std::string, std::string , double> tempTuple(nodeA, nodeB, i);
                edgeVec.push_back(tempTuple);
            }
        }
    }

    //If the Graph is Undirected, post-process to delete duplicates ie (nodeA,nodeB,w) and (nodeB, nodeA,w)
    if (!directed)
    {
        std::vector<std::tuple<std::string, std::string, double>> deleteTheseEdges;
        for (auto edge : edgeVec)
        {
            std::string nodeA = std::get<0>(edge);
            std::string nodeB = std::get<1>(edge);
            double weight = std::get<2>(edge);
            std::tuple<std::string, std::string, double> deleteMe(nodeA, nodeB, weight);
            if (nodeA > nodeB) //Prevents deleting both duplicates, we just want to delete one to leave a unique edge.
            {
                deleteTheseEdges.push_back(deleteMe);
            }
        }
            for (auto edge : deleteTheseEdges)
            {
                edgeVec.erase(std::remove(edgeVec.begin(), edgeVec.end(), edge));
            }
    }
    return edgeVec;
}

int Graph::numNodes()
{
    return nodeMap.size();
}

int Graph::numEdges()
{
    return getEdges().size();
}

bool Graph::nodeExists(std::string name)
{
    return (nodeMap.find(name) != nodeMap.end());
}