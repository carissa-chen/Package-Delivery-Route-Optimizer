#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <list> 
#include <limits>
#include "Address.h"
#include "Package.h"
#include "Graph.h"
#include "Optimizer.h"

//defining the name of the starting location
const char START_LOCATION = 'A'; 

std::unordered_map<std::string, Package> packageTable; 
std::queue<Address> pendingAddresses;

//loads data from text files and populates the graph, hash table, and queue
void loadData(Graph& g) 
{
    //building the graph from map data
    std::ifstream mapFile("map_data.txt");
    std::string line;
    
    if (mapFile.is_open()) 
    {
        char nameA, nameB;
        double dist;
        
        //first loop to find and add all unique locations
        while (std::getline(mapFile, line)) 
        {
            std::stringstream ss(line);
            ss >> nameA >> nameB >> dist;
            g.addLocation(nameA);
            g.addLocation(nameB);
        }
        
        //reset file stream to the beginning for edge creation
        mapFile.clear();
        mapFile.seekg(0, std::ios::beg);
        
        //second loop to add all edges
        while (std::getline(mapFile, line)) 
        {
            std::stringstream ss(line);
            ss >> nameA >> nameB >> dist;
            g.addEdge(nameA, nameB, dist);
        }
        
        mapFile.close();
        //std::cout << "Successfully loaded map data and built graph.\n";
    } else 
    {
        std::cerr << "Error: Could not open map_data.txt\n";
    }

    //load package list data to populate hash table and queue
    std::ifstream pkgFile("package_list.csv");
    if (pkgFile.is_open())
    {
        std::string pkgID;
        char destName;
        
        while (std::getline(pkgFile, line)) 
        {
            std::stringstream ss(line);
            std::getline(ss, pkgID, ',');
            ss >> destName;
        
            //skip starting location if it appears as a destination
            if (destName == START_LOCATION) continue; 

            //hash table population
            Package newPkg = {pkgID, {destName}};
            packageTable[pkgID] = newPkg;

            //queue population
            pendingAddresses.push({destName});
            
            //add location to graph if it wasn't defined in the map
            g.addLocation(destName); 
        }
        pkgFile.close();
        //std::cout << "Successfully loaded package data (hash table and queue).\n";
    } else 
    {
        std::cerr << "Error: Could not open package_list.csv\n";
    }
}

int main() 
{
    std::cout << "--- Package Delivery Route Optimizer ---\n";
    
    Graph deliveryMap;
    loadData(deliveryMap);

    //make sure the graph isn't empty before continuing
    if (deliveryMap.size() == 0) 
    {
        std::cerr << "Program halted: Graph is empty.\n";
        return 1;
    }

    //check what's in the queue
    std::cout << "\n--- Pending Addresses ---\n";
    std::queue<Address> debugQueue = pendingAddresses;
    int count = 0;
    while (!debugQueue.empty()) 
    {
        std::cout << "Package " << ++count << " to be delivered to address " << debugQueue.front().name << "\n";
        debugQueue.pop();
    }
    std::cout << "Total packages in queue: " << count << "\n";

    //optimization logic
    Optimizer routeFinder;
    std::list<Address> optimizedRoute = routeFinder.findOptimizedRoute
    (
        deliveryMap, pendingAddresses, START_LOCATION
    );

    //output the final optimzied route
    std::cout << "\n--- Optimized Route ---\n";
    double totalDistance = 0.0;
    
    int stopNumber = 1;
    for (auto it = optimizedRoute.begin(); it != optimizedRoute.end(); ++it) 
    {
        std::string locType = "";
        if (stopNumber == 1 || stopNumber == (int)optimizedRoute.size()) 
        {
            locType = " (START/END)";
        }
        std::cout << stopNumber << ". Stop: " << it->name << locType << "\n";
        stopNumber++;
    }
    
    // NOTE: You will need to calculate the actual total distance by summing
    // the shortest paths between each consecutive stop in the final route.

    std::cout << "Optimization complete.\n";

    return 0;

}
