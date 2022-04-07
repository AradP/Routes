#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include "Node.h"

using namespace std;

/*
* Get array of all available cities by src city
*/
bool* getAllAvailables(int citiesNum, int srcCityNum, Node** routesNetwork);

/*
* Get array of available cities by src city
*/
void GetToTown(Node** routesNetwork, int srcCityNum, bool whiteCities[], bool availableCities[]);

int main()
{
	string citiesRoutesString;
	int citiesNum, routesNum, srcCityNum;
	// Get number of cities and routes
	cout << "Hello! Enter number of cities and number of routes:\n";
	cin >> citiesNum >> routesNum; // TODO: Make sure we get only 2 numbers
	cout << "cities: " << citiesNum << ", routes: " << routesNum << "\n";
	cin.ignore(1, '\n');

	if (routesNum != 0) {
		// Get the routes
		cout << "Great! Enter the routes:\n";
		getline(cin, citiesRoutesString);
	}
	else {
		citiesRoutesString = ";";
	}

	// Get the routes from the input string to ints vector
	istringstream is(citiesRoutesString);
	vector<int> citiesRoutes((istream_iterator<int>(is)), istream_iterator<int>());
	// Print the given numbers (just for debugging)
	//for (int x : citiesRoutes) std::cout << x << ' ';
	//std::cout << std::endl;

	// Get the number of the source city (Moked)
	cout << "Enter number of source city:\n";
	cin >> srcCityNum;
	if (citiesNum < srcCityNum) { // Check over limit
		cout << "invalid input";
		return 1;
	}
	else if (citiesNum < 0 || routesNum < 0 || srcCityNum < 0) { // Check under limit
		cout << "invalid input";
		return 1;
	}
	else if (citiesRoutes.size() != (routesNum * 2)) {
		cout << "invalid input";
		return 1;
	}

	// Initialize the routes network
	Node** routesNetwork = new Node * [citiesNum];

	// Initialize pointer array to NULL
	for (int i = 0; i < citiesNum; ++i) {
		*(routesNetwork + i) = NULL;
	}

	int cityCounter = 0;
	int srcCityRoute;
	for (int currCity : citiesRoutes) {
		// Check the city number is valid
		if (citiesNum < currCity) {
			cout << "invalid input " << citiesNum;
			return 1;
		}
		// This is the source of the route
		if (cityCounter % 2 == 0) {
			srcCityRoute = currCity - 1;
		}
		// This is the destination of the route
		else {
			Node* currCityNode = new Node();
			currCityNode->data = currCity;
			currCityNode->next = NULL;

			// If the node is first
			if (*(routesNetwork + srcCityRoute) == NULL) {
				*(routesNetwork + srcCityRoute) = currCityNode;
			}
			else {
				Node* firstTemp = routesNetwork[srcCityRoute];
				// Find the last node
				while (routesNetwork[srcCityRoute]->next != NULL)
				{
					routesNetwork[srcCityRoute] = routesNetwork[srcCityRoute]->next;
				}
				routesNetwork[srcCityRoute]->next = currCityNode; // TODO: Save the first node tho
				routesNetwork[srcCityRoute] = firstTemp;

			}
		}
		cityCounter++;
	}

	// Print the array of linked list (just for debugging)
	for (int i = 0; i < citiesNum; ++i) {
		Node* temp = *(routesNetwork + i);

		// Linked list number
		cout << i + 1 << "-->\t";

		// Print the Linked List
		while (temp != NULL) {
			cout << temp->data << " ";
			temp = temp->next;
		}

		cout << '\n';
	}

	// Get all the available cities by the given src city
	bool* allAvailables = getAllAvailables(citiesNum, srcCityNum - 1, routesNetwork);

	// Print all the available cities we got
	cout << "All available cities are:\n";
	for (int currAvailableCount = 0; currAvailableCount < citiesNum; currAvailableCount++) {
		// Meaning this city is available
		if (allAvailables[currAvailableCount]) {
			cout << currAvailableCount + 1 << "\n";
		}
	}

	return 0;
}

bool* getAllAvailables(int citiesNum, int srcCityNum, Node** routesNetwork) {
	// Create list of availabe cities to the source city (the Moked)
	bool* availableCities = new bool[citiesNum];

	// Colors array for all the cities - true means the city in the i place is white
	bool* whiteCities = new bool[citiesNum];

	// Init all the cities in the routesNetwork as white
	// TODO: is there an easier way?
	for (int i = 0; i < citiesNum; ++i) {
		whiteCities[i] = true;
		availableCities[i] = false;
	}

	// Call getToTown
	GetToTown(routesNetwork, srcCityNum, whiteCities, availableCities);

	// Return the list of available cities
	return availableCities;
}
void GetToTown(Node** routesNetwork, int srcCityNum, bool whiteCities[], bool availableCities[]) {
	// Make the city black
	whiteCities[srcCityNum] = false;

	// Add the current city to the available cities list
	availableCities[srcCityNum] = true;

	// For each city that is connected to the current city
	while (routesNetwork[srcCityNum] != NULL)
	{
		Node* currCity = *(routesNetwork + srcCityNum);
		if (whiteCities[currCity->data - 1]) {
			// Get the available cities for this city
			//bool* currAvailableCities = GetToTown(routesNetwork, currCity->data - 1, whiteCities, availableCities);
			GetToTown(routesNetwork, currCity->data - 1, whiteCities, availableCities);
		}
		routesNetwork[srcCityNum] = routesNetwork[srcCityNum]->next;
	}
}

