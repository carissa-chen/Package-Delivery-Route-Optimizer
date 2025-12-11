#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Address.h"

struct Edge 
{
    int destinationIndex; //index of the connected location
    double weight;        //distance between nodes
};

class Graph 
{
private:
    std::vector<Address> allLocations; //stores all the locations (nodes)
    std::vector<std::list<Edge>> adjacencyList; //list
    
    //internal hash table to map character name to index for O(1) search
    std::unordered_map<char, int> locationIndexMap; 

public:
    Graph() {}

    //adds a new location (node) to the graph
    void addLocation(char name) 
    {
        if (locationIndexMap.count(name) == 0) 
        {
            Address newAddr = {name};
            int index = allLocations.size();
            allLocations.push_back(newAddr);
            locationIndexMap[name] = index;
            adjacencyList.push_back({}); //initialize an empty list for the new node
        }
    }

    //adds a weighted and undirected edge between two locations
    void addEdge(char nameA, char nameB, double weight) 
    {
        if (locationIndexMap.count(nameA) && locationIndexMap.count(nameB)) 
        {
            int indexA = locationIndexMap.at(nameA);
            int indexB = locationIndexMap.at(nameB);
            
            //add B to A's adjacency list
            adjacencyList[indexA].push_back({indexB, weight});
            //add A to B's adjacency list (undirected graph)
            adjacencyList[indexB].push_back({indexA, weight});
        }
    }

    //helper to get the location index from its character name
    int getLocationIndex(char name) const 
    {
        if (locationIndexMap.count(name)) 
        {
            return locationIndexMap.at(name);
        }
        return -1;
    }

    //help to get the address object from its index
    Address getAddress(int index) const 
    {
        if (index >= 0 && index < allLocations.size()) 
        {
            return allLocations[index];
        }
        return {' '};
    }

    //helper to get the adjacency list for a given index
    const std::list<Edge>& getEdges(int index) const 
    {
        return adjacencyList[index];
    }
    size_t size() const { return allLocations.size(); }
};

#endif
