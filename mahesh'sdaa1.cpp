

// Route Management

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <stack>
#include <map>
#include <regex>
#include <fstream>
#include <algorithm>


class Vehicle
{
public:
    std::string vehicleModel;
    int loadCapacity;
    bool isAllotted;
    double mileage;
     // Mileage in km/l

    Vehicle(std::string model, int capacity, double mileage)
     {

        vehicleModel = model;
        loadCapacity = capacity;
        isAllotted = false;
        this->mileage = mileage;
    }

};


class Driver
 {

public:
    std::string name;
    int age;
    std::string address;
    bool isAllotted;

    Driver(std::string n, int a, std::string addr)
     {
        name = n;
        age = a;
        address = addr;
        isAllotted = false;
    }

};


class Waste
{

public:
    std::string area;
    int quantity;

    Waste(std::string a, int q)
     {
        area = a;
        quantity = q;
    }

};


// Struct to hold allocation details

struct AllocationDetail
{

    std::string vehicleModel;
    std::string driverName;
    std::string wasteArea;
    double fuelRequired;
    // Added route
    std::vector<std::string> route;

};


class BelgaumWasteManagement
{

private:
    std::vector<Vehicle> vehicles;
    std::vector<Driver> drivers;
    std::vector<Waste> wastes;
    // Graph for routes
    std::vector<std::vector<std::pair<int, int>>> graph;
    // Areas for destination
    std::vector<std::string> areas;
    // Map to store allocations by date
    std::map<std::string, std::vector<AllocationDetail>> allocations;



public:
    BelgaumWasteManagement()

    {
        // Initialize areas and graph
        areas =
         {

            "Kanabargi", "Belgaum", "Shivaji Nagar", "Tilakwadi", "Chennamma Nagar",

            "Gandhinagar", "APMC Yard", "City Market", "Fort Road", "Engg College Road",

            "Khanapur Road", "Udyambag", "Bogarves", "Goaves", "Hindwadi",

            "Mache", "Malmaruti", "Nemgoa", "Shahpur", "Sambhaji Nagar",

            "Angol", "Balekundri", "Camp", "Dharamnath", "Fort Lake"

        };


        graph =
         {

           // Kanabargi
            {{1, 10}, {2, 15}, {4, 20}},

            // Belgaum
            {{0, 10}, {3, 25}, {5, 15}},

            // Shivaji Nagar
            {{0, 15}, {3, 10}, {6, 20}},

             // Tilakwadi
            {{1, 25}, {2, 10}, {7, 30}},

            // Chennamma Nagar
            {{0, 20}, {5, 25}, {8, 15}},

             // Gandhinagar
            {{1, 15}, {4, 25}, {9, 10}},

             // APMC Yard
            {{2, 20}, {10, 25}},

            // City Market
            {{3, 30}, {11, 15}},

              // Fort Road
            {{4, 15}, {12, 20}},

             // Engg College Road
            {{5, 10}, {13, 30}},

             // Khanapur Road
            {{6, 25}, {14, 35}},

             // Udyambag
            {{7, 15}, {15, 20}},

              // Bogarves
            {{8, 20}, {16, 10}},

              // Goaves
            {{9, 30}, {17, 25}},

              // Hindwadi
            {{10, 35}, {18, 15}},

              // Mache
            {{11, 20}, {19, 10}},

            // Malmaruti
            {{12, 10}, {20, 30}},

            // Nemgoa
            {{13, 25}, {21, 15}},

            // Shahpur
            {{14, 15}, {22, 20}},

            // Sambhaji Nagar
            {{15, 10}, {23, 25}},

              // Angol
            {{16, 30}, {24, 20}},

             // Balekundri
            {{17, 15}},

              // Camp
            {{18, 20}},

              // Dharamnath
            {{19, 25}},

                // Fort Lake
            {{20, 20}}

        };

    }

    // Function to get the index of an area
    int getAreaIndex(const std::string& areaName)
     {

        for (int i = 0; i < areas.size(); i++)
          {
            if (areas[i] == areaName)
            {
                return i;
            }
        }
        // Return -1 if area is not found
        return -1;
    }


    // Add Vehicle
    void addVehicle(Vehicle v)
    {
        vehicles.push_back(v);
    }


    // Add Driver
    void addDriver(Driver d)
    {

        drivers.push_back(d);
    }


    // Add waste
    void addWaste(Waste w)
     {
        wastes.push_back(w);
    }


    // Assuming constant speed in km/h for simplicity (40 km/h)
    // average

    const double VEHICLE_SPEED = 40.0;


    // Function to calculate travel time (in hours) based on distance and speed
    double calculateTravelTime(double distance)
    {
        return distance / VEHICLE_SPEED;
    }


// Dijkstra's Algorithm for finding the shortest path from a source to a destination in a weighted graph
std::vector<int> dijkstra(int source, int destination, std::vector<int>& parent)
{

    // Initialize the number of nodes in the graph
    int n = graph.size();

    // Initialize distance vector with maximum possible values (infinity) to indicate that initially all nodes are unreachable
    std::vector<int> distance(n, std::numeric_limits<int>::max());

    // Initialize the parent vector to store the path information
    parent.assign(n, -1);

    // Distance to the source itself is always 0
    distance[source] = 0;

    // Min-heap priority queue to select the node with the smallest distance
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, source});

    // Continue until all nodes processed or priority queue is empty
    while (!pq.empty())

    {
        // Extract the node with the smallest distance
        auto [dist, node] = pq.top();
        pq.pop();

        // If the current distance is greater than the recorded distance, skip processing
        if (dist > distance[node]) continue;

        // Explore all the neighbors of the current node
        for (const auto& [neighbor, weight] : graph[node])
        {

            // Calculate new distance for the neighbor
            int newDist = distance[node] + weight;

            // If the new distance is shorter, update the distance and parent
            if (newDist < distance[neighbor])
            {
                distance[neighbor] = newDist;
                parent[neighbor] = node;

                // Push the updated distance and neighbor node into the priority queue
                pq.push({newDist, neighbor});
            }

        }

    }

    // Check if the destination is reachable
    if (distance[destination] == std::numeric_limits<int>::max())
    {
        std::cout << "No path exists from " << areas[source] << " to " << areas[destination] << "." << std::endl;
        return {};
    }

    // Return the distance vector containing shortest distances from the source to all nodes
    return distance;
}


// Function to find the longest path using modified DFS (Depth-First Search)
void findLongestPath(int source, std::vector<int>& dist, std::vector<int>& parent)
{

    // Initialize the visited vector to keep track of visited nodes
    std::vector<bool> visited(areas.size(), false);

    // Initialize the distance vector with the minimum possible values (negative infinity)
    dist.assign(areas.size(), std::numeric_limits<int>::min());

    // Initialize the parent vector to keep track of the path
    parent.assign(areas.size(), -1);

    // Distance to the source itself is always 0
    dist[source] = 0;

    // Perform DFS from each node to find the longest path
    for (int i = 0; i < areas.size(); i++)
    {
        // Start DFS from unvisited nodes
        if (!visited[i])
        {
            dfsLongestPath(i, visited, dist, parent);
        }
    }

}


// Helper function for DFS to find the longest path
void dfsLongestPath(int u, std::vector<bool>& visited, std::vector<int>& dist, std::vector<int>& parent)
{
    // Mark the current node as visited
    visited[u] = true;

    // Explore all adjacent nodes (neighbors)
    for (const auto& [v, weight] : graph[u])
    {
        // Check if the path through the current node u is longer
        if (dist[u] + weight > dist[v])
        {
            // Update the distance and parent for the neighbor node
            dist[v] = dist[u] + weight;
            parent[v] = u;
        }

        // Perform DFS on unvisited neighbors
        if (!visited[v])
        {
            dfsLongestPath(v, visited, dist, parent);
        }
    }
}


    // Function to validate date format (dd/mm/yyyy)
  bool isValidDate(const std::string& date)
{
    // Define a regular expression pattern to match dates in the format dd/mm/yyyy
    // \b asserts a word boundary to ensure the date is a complete word
    // (0[1-9]|[12][0-9]|3[01]) matches the day part, allowing values from 01 to 31
    // (0[1-9]|1[0-2]) matches the month part, allowing values from 01 to 12
    // ([0-9]{4}) matches the year part, allowing any four-digit number
    const std::regex pattern(R"(\b(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/([0-9]{4})\b)");

    // Use std::regex_match to check if the input date matches the pattern
    return std::regex_match(date, pattern);

}


    // Function to allot a vehicle for a waste collection task
    void allotVehicle(int wasteIndex)
    {

        if (wasteIndex < wastes.size())
          {
            int requiredCapacity = wastes[wasteIndex].quantity;
            std::string date;
            std::cout << "Enter the date for allotment (dd/mm/yyyy): ";
            std::cin >> date;
            while (!isValidDate(date))
             {

                std::cout << "Invalid date format. Please enter the date in the format dd/mm/yyyy: ";
                std::cin >> date;
            }

            for (int i = 0; i < vehicles.size(); i++)
              {
                if (vehicles[i].loadCapacity >= requiredCapacity && !vehicles[i].isAllotted)
                {
                    vehicles[i].isAllotted = true;
             // Assuming first driver is allotted for simplicity
                    drivers[0].isAllotted = true;
                    std::cout << "Vehicle allotted : " << vehicles[i].vehicleModel << std::endl;
                    std::cout << "  Driver name : " << drivers[0].name << " from " << drivers[0].address
                              << " for waste in " << wastes[wasteIndex].area << " on " << date << std::endl;
                    std::cout << " Driver address :" << drivers[0].address << std::endl;
                    std::cout << "Route details from Kanabargi to " << wastes[wasteIndex].area << ":\n";
                    int source = 0;  // Kanabargi
                    int destination = getAreaIndex(wastes[wasteIndex].area);

                    if (destination != -1)

                      {
                        std::vector<int> parent;
                        std::vector<int> distance = dijkstra(source, destination, parent);
                        if (distance[destination] == std::numeric_limits<int>::max())

                        {
                            std::cout << "No route found to the destination." << std::endl;
                            return;
                        }

                        std::stack<int> path;
                        std::vector<std::string> route;
                        for (int at = destination; at != -1; at = parent[at])

                          {
                            path.push(at);
                        }
                        while (!path.empty())

                        {
                            std::cout << areas[path.top()] << (path.size() > 1 ? " -> " : "");
                            route.push_back(areas[path.top()]);
                            path.pop();

                        }
                        std::cout << std::endl;

                        double totalDistance = distance[destination];
                        double travelTime = calculateTravelTime(totalDistance);
                        int hours = static_cast<int>(travelTime);
                        int minutes = static_cast<int>((travelTime - hours) * 60);

                        std::cout << "Total Distance: " << totalDistance << " km\n";

                        std::cout << "This vehicle will be free in " << hours << " hours and " << minutes << " minutes." << std::endl;

                        double fuelRequired = totalDistance / vehicles[i].mileage;

                        std::cout << "Fuel Required: " << fuelRequired << " liters\n";

                        allocations[date].push_back({vehicles[i].vehicleModel, drivers[0].name, wastes[wasteIndex].area, fuelRequired, route});

                        std::cout << " =========================================================" << std::endl;

                    }
                    else
                      {
                        std::cout << "Error: Invalid destination area." << std::endl;
                    }

                    return;
                }
            }

            std::cout << "No suitable vehicle available." << std::endl;
        }
        else
          {
            std::cout << "Invalid waste index." << std::endl;
        }

    }



void displayCollectedWasteByDate() {
    // Check if there are any waste collection records
    if (allocations.empty()) {
        std::cout << "No waste collection details available.\n";
        return;
    }

    std::cout << "=========================================================\n";
    std::cout << "             Waste Collection Details by Date            \n";
    std::cout << "=========================================================\n";

    // Iterate through each allocation date and its details
    for (const auto& [date, details] : allocations) {
        std::cout << "\nDate: " << date << "\n";
        std::cout << "---------------------------------------------------------\n";

        // Display all details for a specific date
        for (const auto& detail : details) {
            std::cout << "  Vehicle Model    : " << detail.vehicleModel << "\n";
            std::cout << "  Driver Name      : " << detail.driverName << "\n";
            std::cout << "  Waste Area       : " << detail.wasteArea << "\n";
            std::cout << "  Fuel Required    : " << detail.fuelRequired << " liters\n";
            std::cout << "  Route            : ";

            // Print the route with arrows between areas
            for (size_t i = 0; i < detail.route.size(); ++i) {
                std::cout << detail.route[i];
                if (i != detail.route.size() - 1) {
                    std::cout << " -> ";
                }
            }
            std::cout << "\n---------------------------------------------------------\n";
        }

        std::cout << "\n=========================================================\n";
    }

    // Summary or concluding message (optional)
    std::cout << "End of waste collection records.\n";
}

void storeDataInFile()
{

    // Hardcoded file path
    std::string filename = "C:\\Users\\mahes\\OneDrive\\Desktop\\route management\\dateDATA.txt";

    // Attempt to open the file for writing
    std::ofstream outFile(filename, std::ios::out | std::ios::app);
     // Open in append mode
    if (!outFile)
      {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    // Write the header to the file
    outFile << "Collected Waste Data by Date:\n";

    // Check if there are any allocations
    if (allocations.empty())
      {
        outFile << "No waste collection data available.\n";
        std::cout << "No data to store in the file.\n";
        outFile.close(); // Close the file before returning
        return;
    }

    // Iterate over all waste collection details by date
    for (const auto& [date, details] : allocations)
      {

        outFile << "Date: " << date << "\n";
        for (const auto& detail : details)
        {

            outFile << "Vehicle Model: " << detail.vehicleModel
                    << ", Driver Name: " << detail.driverName
                    << ", Waste Area: " << detail.wasteArea
                    << ", Fuel Required: " << detail.fuelRequired << " liters\n";
            outFile << "Route: ";

            for (const auto& area : detail.route)
              {
                outFile << area << (area == detail.route.back() ? "" : " -> ");
            }
            outFile << "\n";
        }

        outFile << " ---------------------------------------------------------\n";
    }

    // Close the file and notify the user
    outFile.close();
    std::cout << "Data stored successfully in " << filename << std::endl;

}

//display the content of file

void displayFileContent(const std::string& filename)
{

    // Attempt to open the file for reading
    std::ifstream inFile(filename);
    if (!inFile)
      {
        std::cerr << "Error: Unable to open file '" << filename << "' for reading.\n";
        return;
    }

    // Display a header for better clarity
    std::cout << "=========================================================\n";
    std::cout << "                 Displaying File Content                 \n";
    std::cout << "=========================================================\n";

    // Read and display file content line by line

    std::string line;
    bool isEmpty = true;
    while (std::getline(inFile, line))

      {
        isEmpty = false;
        std::cout << line << "\n";
    }

    // Check if the file was empty
    if (isEmpty)
      {
        std::cout << "The file is empty.\n";
    }

    // Footer for better output structure
    std::cout << "=========================================================\n";
    std::cout << "                  End of File Content                    \n";
    std::cout << "=========================================================\n";

    // Close the file
    inFile.close();

}

//load content of file
// display allocation record

void loadDataFromFile(const std::string& filename)
 {
    // Attempt to open the file for reading
    std::ifstream inFile(filename);
    if (!inFile)
      {
        std::cerr << "Error: Unable to open file '" << filename << "' for reading.\n";
        return;

    }

    // Display a header for better clarity
    std::cout << "=========================================================\n";
    std::cout << "                 Displaying File Content                 \n";
    std::cout << "=========================================================\n";

    // Read and display file content line by line
    std::string line;
    bool isEmpty = true;
    while (std::getline(inFile, line))
      {

        isEmpty = false;
        std::cout << line << "\n";
    }

    // Check if the file was empty
    if (isEmpty)
      {

        std::cout << "The file is empty.\n";
    }

    // Footer for better output structure
    std::cout << "=========================================================\n";
    std::cout << "                  End of File Content                    \n";
    std::cout << "=========================================================\n";

    // Close the file
    inFile.close();

}

// diplay Menu
    void displayMenu()
     {

        std::cout << " --------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "    " << std::endl;
        std::cout << "Belgaum City Waste Management System" << std::endl;
        std::cout << "1. Allot Vehicle" << std::endl;
        std::cout << "2. Display Vehicles" << std::endl;
        std::cout << "3. Display Drivers" << std::endl;
        std::cout << "4. Display Wastes" << std::endl;
        std::cout << "5. Shortest Route Details" << std::endl;
        std::cout << "6. Longest Route Details" << std::endl;
        std::cout << "7. View Available Vehicles and Drivers" << std::endl;
        std::cout << "8. Display Collected Waste by Date" << std::endl;
        std::cout << "9. Save Collected Waste Data to File" << std::endl;
        std::cout << "10. Load Data from File" << std::endl;
        std::cout << "11. Exit" << std::endl;
        std::cout << " ---------------------------------------------------------------------------------------------------------" << std::endl;


    }


void viewAvailableVehiclesAndDrivers()
{

    // Display header for available vehicles
    std::cout << "=========================================================\n";
    std::cout << "                   Available Vehicles                   \n";
    std::cout << "=========================================================\n";

    bool foundAvailableVehicle = false;
    for (size_t i = 0; i < vehicles.size(); i++)
      {

        if (!vehicles[i].isAllotted)
        {
            std::cout << "Vehicle Model: " << vehicles[i].vehicleModel << "\n"
                      << "  Load Capacity: " << vehicles[i].loadCapacity << " kg\n"
                      << "  Mileage: " << vehicles[i].mileage << " km/l\n";
            std::cout << "---------------------------------------------------------\n";
            foundAvailableVehicle = true;
        }
    }

    if (!foundAvailableVehicle)
      {
        std::cout << "No available vehicles at the moment.\n";
    }

    // Display header for available drivers
    std::cout << "\n=========================================================\n";
    std::cout << "                   Available Drivers                    \n";
    std::cout << "=========================================================\n";


    bool foundAvailableDriver = false;
    for (size_t i = 0; i < drivers.size(); i++)
      {
        if (!drivers[i].isAllotted)
          {

            std::cout << "Driver Name: " << drivers[i].name << "\n"
                      << "  Age: " << drivers[i].age << "\n"
                      << "  Address: " << drivers[i].address << "\n";
            std::cout << "---------------------------------------------------------\n";
            foundAvailableDriver = true;
        }
    }

    if (!foundAvailableDriver)
      {

        std::cout << "No available drivers at the moment.\n";
    }

    // Footer for better structure
    std::cout << "=========================================================\n";
    std::cout << "                  End of Availability                   \n";
    std::cout << "=========================================================\n";

}

void displayVehicles()
{

    // Display header for the vehicles section
    std::cout << "=========================================================\n";
    std::cout << "                    List of Vehicles                    \n";
    std::cout << "=========================================================\n";

    if (vehicles.empty())
      {

        std::cout << "No vehicles available in the system.\n";
    }
    else
      {

        for (int i = 0; i < vehicles.size(); i++)

        {
            std::cout << "---------------------------------------------------------\n";
            std::cout << "Vehicle " << i + 1 << ":\n";
            std::cout << "  Model: " << vehicles[i].vehicleModel << "\n"
                      << "  Load Capacity: " << vehicles[i].loadCapacity << " kg\n"
                      << "  Mileage: " << vehicles[i].mileage << " km/l\n"
                      << "  Allotted: " << (vehicles[i].isAllotted ? "Yes" : "No") << "\n";
            std::cout << "---------------------------------------------------------\n";
        }

    }

    // Display footer for the vehicles section
    std::cout << "=========================================================\n";
    std::cout << "                  End of Vehicle List                   \n";
    std::cout << "=========================================================\n";
}


void displayDrivers()
{
    // Display header for the drivers section
    std::cout << "=========================================================\n";
    std::cout << "                    List of Drivers                     \n";
    std::cout << "=========================================================\n";

    if (drivers.empty())
      {

        std::cout << "No drivers available in the system.\n";
    }
    else
      {
        for (int i = 0; i < drivers.size(); i++)
         {
            std::cout << "---------------------------------------------------------\n";
            std::cout << "Driver " << i + 1 << ":\n";
            std::cout << "  Name: " << drivers[i].name << "\n"
                      << "  Age: " << drivers[i].age << " years\n"
                      << "  Address: " << drivers[i].address << "\n";
            std::cout << "---------------------------------------------------------\n";
        }

    }

    // Display footer for the drivers section
    std::cout << "=========================================================\n";
    std::cout << "                  End of Driver List                    \n";
    std::cout << "=========================================================\n";

}

void displayWastes()
{

    // Display header for the wastes section
    std::cout << "=========================================================\n";
    std::cout << "                    Waste Information                    \n";
    std::cout << "=========================================================\n";

    if (wastes.empty())
      {

        std::cout << "No waste data available.\n";
    }
    else
      {
        for (int i = 0; i < wastes.size(); i++)
          {
            std::cout << "---------------------------------------------------------\n";
            std::cout << "Waste " << i + 1 << ":\n";
            std::cout << "  Area: " << wastes[i].area << "\n"
                      << "  Quantity: " << wastes[i].quantity << " kg\n";
            std::cout << "---------------------------------------------------------\n";
        }

    }

    // Display footer for the wastes section
    std::cout << "=========================================================\n";
    std::cout << "                   End of Waste List                    \n";
    std::cout << "=========================================================\n";
}


void findLongestRouteToDestination(int source, int destination)
{

    // Vector to store parent nodes and distances from the source node

    std::vector<int> parent;
    std::vector<int> dist;

    // Call findLongestPath to compute the longest path from the source

    findLongestPath(source, dist, parent);

    // Check if the destination is reachable from the source
    if (dist[destination] == std::numeric_limits<int>::min())
      {

        std::cout << "No path exists from " << areas[source] << " to " << areas[destination] << "." << std::endl;
        return;
    }

    // Display the longest distance to the destination
    std::cout << "Longest distance to " << areas[destination] << ": " << dist[destination] << " km" << std::endl;

    // Reconstruct and display the longest path

}

 // Existing code...

    // to simulate vehicle maintenance
    void performVehicleMaintenance()
    {
        // Placeholder for maintenance logic
    }

    //  to simulate driver training
    void conductDriverTraining()
    {
        // Placeholder for training logic
    }

    //  to simulate waste segregation
    void segregateWaste()
    {
        // Placeholder for waste segregation logic
    }

    //  simulate fuel efficiency calculation
    void calculateFuelEfficiency()
    {
        // Placeholder for fuel efficiency calculation logic
    }

    // to simulate route optimization
    void optimizeRoutes()
    {
        // Placeholder for route optimization logic
    }

    // to simulate waste disposal reporting
    void generateWasteDisposalReport()
    {
        // Placeholder for report generation logic
    }

    //  to simulate emergency response
    void handleEmergencySituation()
    {
        // Placeholder for emergency handling logic
    }

    //  to simulate vehicle tracking
    void trackVehicles()
    {
        // Placeholder for vehicle tracking logic
    }

    // to simulate driver feedback collection
    void collectDriverFeedback()
    {
        // Placeholder for feedback collection logic
    }

    // to simulate waste collection scheduling
    void scheduleWasteCollection()
    {
        // Placeholder for scheduling logic
    }

    // all remaining functions

//calculate shortest route
// use of dijkastra algorithm
void findShortestRouteToDestination(int source, int destination)
{
    std::vector<int> parent;

    // Compute the shortest distances and parent paths using Dijkstra's algorithm
    auto distances = dijkstra(source, destination, parent);

    // Check if the destination is reachable from the source
    if (distances[destination] == std::numeric_limits<int>::max())
      {

        std::cout << "No path exists from " << areas[source] << " to " << areas[destination] << "." << std::endl;
        return;
    }

    // Display the shortest distance to the destination
    std::cout << "Shortest distance to " << areas[destination] << ": " << distances[destination] << " km" << std::endl;

    // Display the shortest route from source to destination
    std::cout << "Route: ";
    std::stack<int> path;
    for (int at = destination; at != -1; at = parent[at])
      {
        path.push(at);
    }

    // Output the path from source to destination in human-readable format
    while (!path.empty())
    {

        std::cout << areas[path.top()] << (path.size() > 1 ? " -> " : "");
        path.pop();
    }

    std::cout << std::endl;

    // Calculate and display additional travel information
    //in hr and min
    double totalDistance = distances[destination];
    double travelTime = calculateTravelTime(totalDistance);

    // Extract hours and minutes from the travel time
    int hours = static_cast<int>(travelTime);
    int minutes = static_cast<int>((travelTime - hours) * 60);

    // Display the travel time and other relevant information
    std::cout << " =========================================================" << std::endl;
    std::cout << "Total Distance: " << totalDistance << " km\n";

    // Display the estimated travel time in hours and minutes
    std::cout << "Estimated Travel Time: " << hours << " hours and " << minutes << " minutes." << std::endl;

    // Additional informative output for the user
    std::cout << "\nPlease ensure the vehicle is ready for travel and has sufficient fuel." << std::endl;
    std::cout << "Remember to check the weather and road conditions before starting the journey." << std::endl;

    // Conclusion message
    std::cout << " =========================================================" << std::endl;

}

};


int main()

{
    BelgaumWasteManagement bwm;

    // Add Vehicles
    //vehicle1
    bwm.addVehicle(Vehicle("Tata Ace", 1000, 13.0));
    //vehicle2
    bwm.addVehicle(Vehicle("Mahindra Pickup", 2000, 12.0));
    //vehicle3
    bwm.addVehicle(Vehicle("Ashok Leyland Dost", 3000, 14.0));
    //vehicle4
    bwm.addVehicle(Vehicle("BharatBenz 1160", 4000, 11.0));
    //vehicle5
    bwm.addVehicle(Vehicle("Eicher 950", 6000, 12.0));
    //vehicle6
    bwm.addVehicle(Vehicle("Tata Ace Hybrid", 9000, 11.0));
    //vehicle7
    bwm.addVehicle(Vehicle("Tata Force", 10000, 10.0));
    //vehicle8
    bwm.addVehicle(Vehicle("Leyland Bada Dost", 11000, 9.0));
    //vehicle9
    bwm.addVehicle(Vehicle("MAN 1600", 12000, 8.0));
    //vehicle10
    bwm.addVehicle(Vehicle("Tata 1630", 14000, 7.0));
    //All vehicle


    // Add Drivers



    // All drivers

    bwm.addDriver(Driver("Abhi", 30, "Gokak"));
    bwm.addDriver(Driver("Arun", 32, "Hubli"));
    bwm.addDriver(Driver("Akash", 25, "Kanabargi"));
    bwm.addDriver(Driver("Gagan", 28, "Sambre"));
    bwm.addDriver(Driver("Venki", 26, "Suldal"));

    // Add Wastes
    //wastearea1
    bwm.addWaste(Waste("Kanabargi", 500));

    //wastearea2
    bwm.addWaste(Waste("BelgaumCBT", 1000));


    //wastearea3
    bwm.addWaste(Waste("Shivaji Nagar", 1500));
    //wastearea4
    bwm.addWaste(Waste("Tilakwadi", 2000));

    //wastearea5
    bwm.addWaste(Waste("Chennamma Nagar", 2500));

    //wastearea6
    bwm.addWaste(Waste("Gandhinagar", 3000));

    //wastearea7
    bwm.addWaste(Waste("APMC Yard", 3500));

    //wastearea8
    bwm.addWaste(Waste("City Market", 4000));

    //wastearea9
    bwm.addWaste(Waste("Fort Road", 4500));

    //wastearea10
    bwm.addWaste(Waste("Engg College Road", 5000));

    //wastearea11
    bwm.addWaste(Waste("Khanapur Road", 5500));

    //wastearea12
    bwm.addWaste(Waste("Udyambag", 6000));

    //wastearea13
    bwm.addWaste(Waste("Bogarves", 6500));


    //wastearea14
    bwm.addWaste(Waste("Goaves", 7000));

    //wastearea15
    bwm.addWaste(Waste("Hindwadi", 7500));

    //wastearea16
    bwm.addWaste(Waste("Mache", 8000));

    //wastearea17
    bwm.addWaste(Waste("Malmaruti", 8500));

    //wastearea18
    bwm.addWaste(Waste("Nemgoa", 9000));

    //wastearea19
    bwm.addWaste(Waste("Shahpur", 9500));

    //wastearea20
    bwm.addWaste(Waste("Sambhaji Nagar", 10000));

    //wastearea21
    bwm.addWaste(Waste("Angol", 10500));

    //wastearea22
    bwm.addWaste(Waste("Balekundri", 11000));

    //wastearea23
    bwm.addWaste(Waste("Camp", 11500));
    //wastearea24
    bwm.addWaste(Waste("Dharamnath", 12000));

    //wastearea25
    bwm.addWaste(Waste("Fort Lake", 12500));


    int choice;

    do
      {

        bwm.displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;


        // Handle invalid input for menu choice
        if(std::cin.fail())
          {
            std::cin.clear();
             // clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // ignore invalid input
            std::cout << "Invalid input. Please enter a valid number.\n";
            continue;

        }

        switch (choice)
         {

        case 1:
          {

            int wasteIndex;
             std::cout << "Enter destination area index:\n";
              std::cout << "1: Kanabargi\n";
              std::cout << "2: Belgaum\n";
              std::cout << "3: Shivaji Nagar\n";
              std::cout << "4: Tilakwadi\n";
              std::cout << "5: Chennamma Nagar\n";
              std::cout << "6: Gandhinagar\n";
              std::cout << "7: APMC Yard\n";
              std::cout << "8: City Market\n";
              std::cout << "9: Fort Road\n";
              std::cout << "10: Engg College Road\n";
              std::cout << "11: Khanapur Road\n";
              std::cout << "12: Udyambag\n";
              std::cout << "13: Bogarves\n";
              std::cout << "14: Goaves\n";
              std::cout << "15: Hindwadi\n";
              std::cout << "16: Mache\n";
              std::cout << "17: Malmaruti\n";
              std::cout << "18: Nemgoa\n";
              std::cout << "19: Shahpur\n";
              std::cout << "20: Sambhaji Nagar\n";
              std::cout << "21: Angol\n";
              std::cout << "22: Balekundri\n";
              std::cout << "23: Camp\n";
              std::cout << "24: Dharamnath\n";
              std::cout << "25: Fort Lake\n";
            std::cin >> wasteIndex;
            if(wasteIndex < 1 || wasteIndex > 25)
              {

                std::cout << "Invalid waste index. Please enter a number between 1 and 25.\n";
            }
            else
              {

                bwm.allotVehicle(wasteIndex - 1);
            }

            break;
        }


        case 2:
            bwm.displayVehicles();
            break;


        case 3:
            bwm.displayDrivers();
            break;


        case 4:
            bwm.displayWastes();
            break;


        case 5:
           {

            int destination;
            std::cout << "Enter destination area index (1: Kanabargi, 2: Belgaum, 3: Shivaji Nagar, ... 25: Fort Lake): ";

            std::cin >> destination;

            if(destination < 1 || destination > 25)
              {

                std::cout << "Invalid destination. Please enter a number between 1 and 25.\n";

            }
             else
              {

                int source = 0; // Kanabargi
                bwm.findShortestRouteToDestination(source, destination - 1);
            }
            break;
        }


        case 6:
          {

            int destination;
           std::cout << "Enter destination area index:\n";
              std::cout << "1: Kanabargi\n";
              std::cout << "2: Belgaum\n";
              std::cout << "3: Shivaji Nagar\n";
              std::cout << "4: Tilakwadi\n";
              std::cout << "5: Chennamma Nagar\n";
              std::cout << "6: Gandhinagar\n";
              std::cout << "7: APMC Yard\n";
              std::cout << "8: City Market\n";
              std::cout << "9: Fort Road\n";
              std::cout << "10: Engg College Road\n";
              std::cout << "11: Khanapur Road\n";
              std::cout << "12: Udyambag\n";
              std::cout << "13: Bogarves\n";
              std::cout << "14: Goaves\n";
              std::cout << "15: Hindwadi\n";
              std::cout << "16: Mache\n";
              std::cout << "17: Malmaruti\n";
              std::cout << "18: Nemgoa\n";
              std::cout << "19: Shahpur\n";
              std::cout << "20: Sambhaji Nagar\n";
              std::cout << "21: Angol\n";
              std::cout << "22: Balekundri\n";
              std::cout << "23: Camp\n";
              std::cout << "24: Dharamnath\n";
              std::cout << "25: Fort Lake\n";
            std::cin >> destination;
            if(destination < 1 || destination > 25)
              {

                std::cout << "Invalid destination. Please enter a number between 1 and 25.\n";
            }
            else
              {
                int source = 0;
                // Kanabargi

                bwm.findLongestRouteToDestination(source, destination - 1);
            }

            break;
        }

        case 7:
            bwm.viewAvailableVehiclesAndDrivers();
            break;


        case 8:
            bwm.displayCollectedWasteByDate();
            break;


        case 9:
            bwm.storeDataInFile();

            break;



        case 10:

          {

            std::string filename = "C:\\Users\\mahes\\OneDrive\\Desktop\\route management\\dateDATA.txt";
            //file path


            bwm.loadDataFromFile(filename);
            break;
        }



        case 11:
            std::cout << "Exiting the program." << std::endl;
            break;

        default:

            std::cout << "Invalid choice. Please try again." << std::endl;
        }


    } while (choice != 11);


    return 0;


}




