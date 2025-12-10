#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <limits>
#include "Address.h"
#include "Package.h"
#include "Graph.h"
#include "Optimizer.h"

//helo!
// Define the name of your starting location (Depot)
const char START_LOCATION = 'A'; 

// **HASH TABLE** and **QUEUE** storage
std::unordered_map<std::string, Package> packageTable; 
std::queue<Address> pendingAddresses;

/**
 * Loads data from text files and populates the Graph, Hash Table, and Queue.
 */
void loadData(Graph& g) {
    // --- 1. Load MAP Data (Building the Graph) ---
    std::ifstream mapFile("map_data.txt");
    std::string line;
    
    if (mapFile.is_open()) {
        char nameA, nameB;
        double dist;
        
        // First pass: Discover and add all unique locations
        while (std::getline(mapFile, line)) {
            std::stringstream ss(line);
            ss >> nameA >> nameB >> dist;
            g.addLocation(nameA);
            g.addLocation(nameB);
        }
        
        // Reset file stream to the beginning for edge creation
        mapFile.clear();
        mapFile.seekg(0, std::ios::beg);
        
        // Second pass: Add all edges
        while (std::getline(mapFile, line)) {
            std::stringstream ss(line);
            ss >> nameA >> nameB >> dist;
            g.addEdge(nameA, nameB, dist);
        }
        
        mapFile.close();
        std::cout << "Successfully loaded map data and built Graph.\n";
    } else {
        std::cerr << "Error: Could not open map_data.txt\n";
    }

    // --- 2. Load PACKAGE Data (Populating Hash Table and Queue) ---
    std::ifstream pkgFile("package_list.csv");
    if (pkgFile.is_open()) {
        std::string pkgID;
        char destName;
        
        while (std::getline(pkgFile, line)) {
            std::stringstream ss(line);
            ss >> pkgID >> destName;
            
            // Skip the starting location if it appears as a destination
            if (destName == START_LOCATION) continue; 

            // **HASH TABLE** population
            Package newPkg = {pkgID, {destName}};
            packageTable[pkgID] = newPkg;

            // **QUEUE** population
            pendingAddresses.push({destName});
            
            // Add location to graph if it wasn't defined in the map (just in case)
            g.addLocation(destName); 
        }
        pkgFile.close();
        std::cout << "Successfully loaded package data (Hash Table & Queue).\n";
    } else {
        std::cerr << "Error: Could not open package_list.csv\n";
    }
}

int main() {
    std::cout << "--- Package Delivery Route Optimizer ---\n";
    
    Graph deliveryMap;
    loadData(deliveryMap);

    // Ensure the Graph is not empty before proceeding
    if (deliveryMap.size() == 0) {
        std::cerr << "Program halted: Graph is empty.\n";
        return 1;
    }

    // --- 3. Run Optimization Logic ---
    Optimizer routeFinder;
    std::vector<Address> optimizedRoute = routeFinder.findOptimizedRoute(
        deliveryMap, pendingAddresses, START_LOCATION
    );

    // --- 4. Output the Final Route (The List) ---
    std::cout << "\n--- Optimized Route (Final List) ---\n";
    double totalDistance = 0.0; // Placeholder for actual distance calculation
    
    for (size_t i = 0; i < optimizedRoute.size(); ++i) {
        std::string locType = (i == 0 || i == optimizedRoute.size() - 1) ? " (START/END)" : "";
        std::cout << i + 1 << ". Stop: " << optimizedRoute[i].name << locType << "\n";
    }
    
    // NOTE: You will need to calculate the actual total distance by summing
    // the shortest paths between each consecutive stop in the final route.
    // This calculation is left as an exercise for the student.

    std::cout << "Optimization complete.\n";

    return 0;
}