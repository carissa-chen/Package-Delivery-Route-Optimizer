// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Address.h"

struct Edge {
    int destinationIndex; // Index of the connected location in the allLocations vector
    double weight;        // Distance or time
};

class Graph {
private:
    std::vector<Address> allLocations; // Stores all nodes/locations
    std::vector<std::vector<Edge>> adjacencyList; // The required **LIST** structure
    
    // Internal Hash Table to map character name to index for O(1) lookup
    std::unordered_map<char, int> locationIndexMap; 

public:
    Graph() {}

    // Adds a new location (node) to the graph
    void addLocation(char name) {
        if (locationIndexMap.count(name) == 0) {
            Address newAddr = {name};
            int index = allLocations.size();
            allLocations.push_back(newAddr);
            locationIndexMap[name] = index;
            adjacencyList.push_back({}); // Initialize an empty adjacency list for the new node
        }
    }

    // Adds a weighted, undirected edge between two locations
    void addEdge(char nameA, char nameB, double weight) {
        if (locationIndexMap.count(nameA) && locationIndexMap.count(nameB)) {
            int indexA = locationIndexMap.at(nameA);
            int indexB = locationIndexMap.at(nameB);
            
            // Add B to A's adjacency list
            adjacencyList[indexA].push_back({indexB, weight});
            
            // Add A to B's adjacency list (undirected graph)
            adjacencyList[indexB].push_back({indexA, weight});
        }
    }

    // Helper to get the location index from its character name
    int getLocationIndex(char name) const {
        if (locationIndexMap.count(name)) {
            return locationIndexMap.at(name);
        }
        return -1; // Not found
    }

    // Helper to get the Address object from its index
    Address getAddress(int index) const {
        if (index >= 0 && index < allLocations.size()) {
            return allLocations[index];
        }
        return {' '}; // Return empty address if out of bounds
    }

    // Helper to get the adjacency list for a given index
    const std::vector<Edge>& getEdges(int index) const {
        return adjacencyList[index];
    }
    
    size_t size() const { return allLocations.size(); }
};

#endif // GRAPH_H