#include "Map.h"
#include <iostream>

int main()
{
	vector<Country*>* countries = new vector<Country*>;
	vector<Continent*>* continents = new vector<Continent*>;

	//[continents]
	Continent* continent1 = new Continent("Atlantic");
	Continent* continent2 = new Continent("Quebec");

	//[countries]
	Country* c1 = new Country("NB");
	Country* c2 = new Country("PEI");
	Country* c3 = new Country("NS");
	Country* c4 = new Country("Newfoundland");
	Country* c5 = new Country("Labrador");
	Country* c6 = new Country("QC-North");
	Country* c7 = new Country("QC-Central");
	Country* c8 = new Country("QC-South");
	Country* c9 = new Country("Russia");
	Country* c10 = new Country("China");

	continents->push_back(continent1);
	continents->push_back(continent2);

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

	bool visited[10] = { false };

	Map* map = new Map(continents, countries);

	continent1->add_country(c1);
	continent1->add_country(c2);
	continent1->add_country(c3);
	continent1->add_country(c4);
	continent1->add_country(c5);

	c1->set_continent(continent1);
	c2->set_continent(continent1);
	c3->set_continent(continent1);
	c4->set_continent(continent1);
	c5->set_continent(continent1);

	continent2->add_country(c6);
	continent2->add_country(c7);
	continent2->add_country(c8);

	c6->set_continent(continent2);
	c7->set_continent(continent2);
	c8->set_continent(continent2);

	//[borders]
	c1->add_adjacent_country(c8);
	c1->add_adjacent_country(c2);
	c1->add_adjacent_country(c3);

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
	c6->add_adjacent_country(c7);

	c7->add_adjacent_country(c4);
	c7->add_adjacent_country(c5);
	c7->add_adjacent_country(c6);
	c7->add_adjacent_country(c8);

	c8->add_adjacent_country(c1);
	c8->add_adjacent_country(c7);

	c9->add_adjacent_country(c10);

	c10->add_adjacent_country(c9);
}
