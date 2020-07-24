#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

class Node
{
private:
    double data;
    std::string name;

    std::unordered_map<std::string, std::multiset<double>> *neighborMap;
    std::unordered_set<std::string> neighborOfSet;

public:
    Node(double data, std::string name);
    ~Node();

    void addNeighbor(std::string neighborName, double weight);

    double getData();
    std::unordered_map<std::string, std::multiset<double>> * getMapPtr();
    std::unordered_set<std::string>& getSetRef();
};

#endif