#include "Map.h"

Map::Map() :
        continents_(new vector<Continent*>()),
        countries_(new vector<Country*>()) {}

Map::Map(vector<Continent*>* continents, vector<Country*>* countries) :
	continents_(continents),
	countries_(countries) {}

bool Map::validate_connected_graph() const
{
    //To validate the map is a connected graph, we must ensure that every country is visited. We create
    //an array the size of the countries vector, and perform a DFT to navigate the graph. After all possible
    //paths have been visited, we verify if all the elements of was_visited have been set to true;
	auto was_visited = new vector<bool>(countries_->size(), false);
	depth_first_traversal(countries_->at(0), was_visited);

	for (auto const& b : *was_visited)
	{
		if (!b)
			return false;
	}

	return true;
}

bool Map::validate_continent_singularity()
{
    //Validates that a country belongs to a single continent by checking each continent for the country,
    //and returning false if the country is found in a second continent or if it is not found in any continent.
	for(auto const& country: *countries_)
	{
		bool found_continent = false;

		for(auto const& continent: *continents_)
		{
			if(continent->has_country(country))
			{
				if(found_continent)
				{
					return false; //This happens when the country is found twice
				}

				found_continent = true;
			}
		}

		if(!found_continent)
		{
			return false; //This would mean we didn't find a single continent that had this country
		}
	}

	return true;
}

void Map::depth_first_traversal(Country* country, vector<bool>* visited_countries) const
{
	visited_countries->at(get_country_index(country)) = true; //The current country is now being visited, mark as true.
	for (auto const& c : *country->get_adjacent_countries())
	{
		if (!visited_countries->at(get_country_index(c))) //Do not visit countries that have already been visited.
		{
			depth_first_traversal(c, visited_countries); //Recursive function
		}
	}
}

int Map::get_country_index(Country* country) const
{
	for(int i = 0; i < countries_->size(); i++)
	{
		if(countries_->at(i)->get_name() == country->get_name())
		{
			return i;
		}
	}
	return -1; //Didn't get any index...
}

Continent::Continent(string name) :
	name_(new string(name)),
	countries_(new vector<Country*>()) {}


string Continent::get_name() const
{
	return *name_;
}

vector<Country*>* Continent::get_countries() const
{
	return countries_;
}

bool Continent::has_country(Country* c)
{
	for(auto const& country: *countries_)
	{
		if(c->get_name() == country->get_name())
		{
			return true;
		}
	}

	return false;
}

void Continent::set_name(string* name)
{
	name_ = name;
}

void Continent::depth_first_traversal(Country* country, vector<bool>* visited_countries) const
{
	visited_countries->at(get_country_index(country)) = true;
	for (auto const& c : *country->get_adjacent_countries())
	{
		//In this case, validate that the country is also within the continent.
		if (c->get_continent()->get_name() == *name_ && !visited_countries->at(get_country_index(c)))
		{
			depth_first_traversal(c, visited_countries);
		}
	}
}

	
bool Continent::validate_continent() const
{
	auto* was_visited = new vector<bool>(countries_->size(), false);
	depth_first_traversal(countries_->at(0), was_visited);

	for(auto const& b : *was_visited)
	{
		if (!b)
			return false;
	}

	return true;
}

void Continent::add_country(Country* country) const
{
	if(!already_added(country))
	{
		countries_->push_back(country);
	}
}

int Continent::get_country_index(Country* country) const
{
	for (int i = 0; i < countries_->size(); i++)
	{
		if (countries_->at(i)->get_name() == country->get_name())
		{
			return i;
		}
	}
	return -1; //Didn't get any index...
}

bool Continent::already_added(Country* country) const
{
	for(auto const& c : *countries_)
	{
		if(country == c)
		{
			return true;
		}
	}

	return false;
}

Country::Country(string name, Continent* continent):
	name_(new string(name)),
	continent_(continent),
	player_(nullptr),
	nb_armies_(nullptr),
	adjacent_countries_(new vector<Country*>()) {}


string Country::get_name() const
{
	return *name_;
}

Continent* Country::get_continent() const
{
	return continent_;
}

Player* Country::get_player() const
{
	return player_;
}

vector<Country*>* Country::get_adjacent_countries() const
{
	return adjacent_countries_;
}

int* Country::get_nb_armies() const
{
	return nb_armies_;
}

void Country::set_name(string name)
{
	name_ = new string(name);
}

void Country::set_continent(Continent* continent)
{
	continent_ = continent;
}

void Country::set_player(Player* player)
{
	player_ = player;
}

void Country::set_nb_armies(int* nb_armies)
{
	nb_armies_ = nb_armies;
}

void Country::add_adjacent_country(Country* country) const
{
	if(!already_added(country))
	{
		adjacent_countries_->push_back(country);
	}
}

bool Country::already_added(Country* country) const
{
	for (auto const& c : *adjacent_countries_)
	{
		if (country == c)
		{
			return true;
		}
	}
	return false;
}