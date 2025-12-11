#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include <queue> // Provides std::priority_queue (Min-Heap) and std::queue
#include <limits>
#include <algorithm>
#include "Graph.h"

// Define a structure for Dijkstra's Priority Queue item
struct DijkstraItem {
    double distance;
    int index;

    // Comparator for the Min-Heap (priority queue)
    // We want the item with the SMALLEST distance to have the highest priority
    bool operator>(const DijkstraItem& other) const {
        return distance > other.distance;
    }
};

class Optimizer {
private:
    // **Dijkstra's Algorithm**: Finds the shortest distances from a start node to all others
    std::vector<double> runDijkstra(const Graph& g, int startIndex) {
        int V = g.size();
        std::vector<double> dist(V, std::numeric_limits<double>::infinity());
        
        // **TREE (Min-Heap)** Implementation: Priority Queue
        // Uses the custom operator> to create a Min-Heap based on distance
        std::priority_queue<DijkstraItem, std::vector<DijkstraItem>, std::greater<DijkstraItem>> pq;

        dist[startIndex] = 0;
        pq.push({0.0, startIndex});

        while (!pq.empty()) {
            double d = pq.top().distance;
            int u = pq.top().index;
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : g.getEdges(u)) {
                int v = edge.destinationIndex;
                double weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

public:
    // **Nearest Neighbor Heuristic**: Finds the best route using the calculated shortest paths
    std::vector<Address> findOptimizedRoute(const Graph& g, std::queue<Address> pendingAddresses, char startLocationName) {
    
    int V = g.size();
    std::vector<Address> finalRoute;
    
    // turn queue to a set of unvisited destinations
    std::unordered_map<char, bool> visited; 
    std::queue<Address> tempQueue = pendingAddresses; // copy to preserve original
    
    while (!tempQueue.empty()) {
        visited[tempQueue.front().name] = false; // initialize as unvisited
        tempQueue.pop();
    }
    
    // 2. Start at the initial location
    char currentLocName = startLocationName;
    finalRoute.push_back(g.getAddress(g.getLocationIndex(currentLocName)));

    // 3. Visit all destinations using nearest neighbor
    while (!visited.empty()) {
        // Run Dijkstra from current location
        int currentLocIndex = g.getLocationIndex(currentLocName);
        std::vector<double> distances = runDijkstra(g, currentLocIndex);

        double minDistance = std::numeric_limits<double>::infinity();
        char nextLocName = ' ';

        // find the closest unvisited destination
        for (auto& pair : visited) {
            char loc = pair.first;
            bool isVisited = pair.second;
            
            if (!isVisited) {  // now can actually find unvisited ones
                int locIndex = g.getLocationIndex(loc);
                if (distances[locIndex] < minDistance) {
                    minDistance = distances[locIndex];
                    nextLocName = loc;
                }
            }
        }
        
        // If a next location was found, visit it
        if (nextLocName != ' ') {
            currentLocName = nextLocName;
            finalRoute.push_back(g.getAddress(g.getLocationIndex(currentLocName)));
            visited[currentLocName] = true; // mark as visited
            
            // Remove from map once visited
            if (visited[currentLocName]) {
                visited.erase(currentLocName);
            }
        } else {
            break; // No reachable unvisited location found
        }
    }

    // 4. Return to the starting location
    finalRoute.push_back(g.getAddress(g.getLocationIndex(startLocationName)));
    
    return finalRoute;
}
};

#endif // OPTIMIZER_H