#include "Map.h"
#include <iostream>

int main()
{
	//[continents]
	Continent* continent1 = new Continent("Maritimes");
	Continent* continent2 = new Continent("Quebec");
	Continent* continent3 = new Continent("Ontario");

	vector<Continent*>* continents = new vector<Continent*>;
	continents->push_back(continent1);
	continents->push_back(continent2);
	continents->push_back(continent3);

	//[countries]
	Country* c1 = new Country("NB", continent1);
	Country* c2 = new Country("PEI", continent1);
	Country* c3 = new Country("NS", continent1);
	Country* c4 = new Country("Newfoundland", continent1);
	Country* c5 = new Country("Labrador", continent1);

	Country* c6 = new Country("QC-North", continent2);
	Country* c7 = new Country("QC-Central", continent2);
	Country* c8 = new Country("QC-South", continent2);

	Country* c9 = new Country("Ottawa", continent3);
	Country* c10 = new Country("Toronto", continent3);

	vector<Country*>* countries = new vector<Country*>;
	countries->push_back(c1);
	countries->push_back(c2);
	countries->push_back(c3);
	countries->push_back(c4);
	countries->push_back(c5);
	countries->push_back(c6);
	countries->push_back(c7);
	countries->push_back(c8);
	countries->push_back(c9);
	countries->push_back(c10);

	continent1->add_country(c1);
	continent1->add_country(c2);
	continent1->add_country(c3);
	continent1->add_country(c4);
	continent1->add_country(c5);
	//continent1->add_country(c6); //Test case 3: Uncomment this. (Adds a country to a second continent)

	continent2->add_country(c6);
	continent2->add_country(c7);
	continent2->add_country(c8);

	continent3->add_country(c9);
	continent3->add_country(c10);

	//[borders]
	c1->add_adjacent_country(c2);
	c1->add_adjacent_country(c3);
	c1->add_adjacent_country(c8);

	c2->add_adjacent_country(c1);
	c2->add_adjacent_country(c3);

	c3->add_adjacent_country(c1);
	c3->add_adjacent_country(c2);
	c3->add_adjacent_country(c4);

	c4->add_adjacent_country(c3);
	c4->add_adjacent_country(c5);
	c4->add_adjacent_country(c7);

	c5->add_adjacent_country(c4);
	c5->add_adjacent_country(c6);
	c5->add_adjacent_country(c7);

	c6->add_adjacent_country(c5);
	c6->add_adjacent_country(c7); //Test case 2: Comment this out. (Breaks QC-North off of the rest of Quebec, forces you to pass by Maritimes, invalid)

	c7->add_adjacent_country(c4);
	c7->add_adjacent_country(c5);
	c7->add_adjacent_country(c6); //Test case 2: Comment this out. (Breaks QC-North off of the rest of Quebec, forces you to pass by Maritimes, invalid)
	c7->add_adjacent_country(c8);

	c8->add_adjacent_country(c7);
	c8->add_adjacent_country(c9); //Test case 1: Comment this out. (Breaks Ontario off of Quebec)

	c9->add_adjacent_country(c8); //Test case 1: Comment this out. (Breaks Ontario off of Quebec)
	c9->add_adjacent_country(c10);

	c10->add_adjacent_country(c9);

	//Create map
	Map* map = new Map(continents, countries);

	//Verifying Maritimes
	cout << "verifying Maritimes is a subgraph..." << endl;
	if (continent1->validate_continent())
	{
		cout << "Valid continent." << endl;
	}
	else
	{
		cout << "Invalid continent." << endl;
	}

	cout << endl;

	//Verifying Quebec
	cout << "verifying Quebec is a subgraph..." << endl;
	if(continent2->validate_continent())
	{
		cout << "Valid continent." << endl;
	} else
	{
		cout << "Invalid continent." << endl;
	}

	cout << endl;

	//Verifying Ontario
	cout << "verifying Ontario is a subgraph..." << endl;
	if (continent3->validate_continent())
	{
		cout << "Valid continent." << endl;
	}
	else
	{
		cout << "Invalid continent." << endl;
	}

	cout << endl;

	//Verifying Map
	cout << "verifying entire map is a connected graph..." << endl;
	if (map->validate_connected_graph())
	{
		cout << "Valid map." << endl;
	}
	else
	{
		cout << "Invalid map." << endl;
	}

	cout << endl;

	//Verifying Continent singularity
	cout << "verifying each country has a single continent..." << endl;
	if (map->validate_continent_singularity())
	{
		cout << "Valid continent singularity." << endl;
	}
	else
	{
		cout << "Invalid continent singularity." << endl;
	}
}
