#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>

std::string edgeString(std::tuple<std::string, std::string, int>);

int main()
{
    std::vector<std::string> nodes {"A", "B", "C", "D"};
    Graph G;
    G.addNode(2, "A");
    G.addNode(1, "B");
    G.addNode(4, "C");
    G.addNode(7, "D");

    G.addEdge("A","B", 1);
    G.addEdge("B","C", 8);
    G.addEdge("C","D", 9);
    G.addEdge("D","A", 6);

    std::cout << G.getInfo();

    std::cout << "\n------------------------------------" << '\n';
    std::cout << "Done." << '\n';
    std::cout << "------------------------------------" << '\n';

    return 0;
}

std::string edgeString(std::tuple<std::string, std::string, int> edge)
{
    std::string str = std::get<0>(edge) + std::get<1>(edge) + std::to_string(std::get<2>(edge));
    return str;
}