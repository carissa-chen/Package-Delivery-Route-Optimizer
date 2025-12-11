#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "Graph.h"

//define structure for Dijkstra's priority queue item
struct DijkstraItem 
{
    double distance;
    int index;

    //comparator for the min heap (priority queue)
    //using a min heap because we want the item with the smalest distance to have the highest priority
    bool operator>(const DijkstraItem& other) const 
    {
        return distance > other.distance;
    }
};

class Optimizer 
{
private:
    //Dijkstra's algorithm to find the shortest distances from a start node to all others
    std::vector<double> runDijkstra(const Graph& g, int startIndex) 
    {
        int V = g.size();
        std::vector<double> dist(V, std::numeric_limits<double>::infinity());
        
        //min heap implementation
        std::priority_queue<DijkstraItem, std::vector<DijkstraItem>, std::greater<DijkstraItem>> pq;

        dist[startIndex] = 0;
        pq.push({0.0, startIndex});

        while (!pq.empty()) 
        {
            double d = pq.top().distance;
            int u = pq.top().index;
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& edge : g.getEdges(u)) 
            {
                int v = edge.destinationIndex;
                double weight = edge.weight;

                if (dist[u] + weight < dist[v]) 
                {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

public:
    //finds the best route using the calculated shortest paths
    std::list<Address> findOptimizedRoute(const Graph& g, std::queue<Address> pendingAddresses, char startLocationName) 
    {
    
    int V = g.size();
    std::list<Address> finalRoute;
    
    //turn queue to a set of unvisited destinations
    std::unordered_map<char, bool> visited; 
    std::queue<Address> tempQueue = pendingAddresses; //copy to preserve original
    
    while (!tempQueue.empty()) 
    {
        visited[tempQueue.front().name] = false; //initialize as unvisited
        tempQueue.pop();
    }
    
    //start at the initial location
    char currentLocName = startLocationName;
    finalRoute.push_back(g.getAddress(g.getLocationIndex(currentLocName)));

    //visit all destinations using nearest neighbor
    while (!visited.empty()) 
    {
        //run Dijkstra algorithm from current location
        int currentLocIndex = g.getLocationIndex(currentLocName);
        std::vector<double> distances = runDijkstra(g, currentLocIndex);

        double minDistance = std::numeric_limits<double>::infinity();
        char nextLocName = ' ';

        //find the closest unvisited destination
        for (auto& pair : visited) 
        {
            char loc = pair.first;
            bool isVisited = pair.second;
            
            if (!isVisited) 
            {  //now we can actually find unvisited ones
                int locIndex = g.getLocationIndex(loc);
                if (distances[locIndex] < minDistance) 
                {
                    minDistance = distances[locIndex];
                    nextLocName = loc;
                }
            }
        }
        
        //if a next location was found visit it
        if (nextLocName != ' ') 
        {
            currentLocName = nextLocName;
            finalRoute.push_back(g.getAddress(g.getLocationIndex(currentLocName)));
            visited[currentLocName] = true; //mark as visited
            
            //remove from map once visited
            if (visited[currentLocName]) 
            {
                visited.erase(currentLocName);
            }
        } else 
        {
            break; //no reachable unvisited location found
        }
    }

    //return to the starting location
    finalRoute.push_back(g.getAddress(g.getLocationIndex(startLocationName)));
    return finalRoute;
}
};

#endif
