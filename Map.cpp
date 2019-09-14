#include "Map.h"

Map::Map()
{
	continents_ = new vector<Continent*>();
	countries_ = new vector<Country*>();
}

Map::Map(vector<Continent*>* continents, vector<Country*>* countries)
{
	continents_ = continents;
	countries_ = countries;
}

void Map::depth_first_traversal(Country* country, bool* visited_countries) const
{
	visited_countries[get_country_index(country)] = true;
	for (auto const& c : *country->get_adjacent_countries())
	{
		if (!visited_countries[get_country_index(c)])
		{
			depth_first_traversal(c, visited_countries);
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

Continent::Continent()
{
	name_ = nullptr;
	countries_ = new vector<Country*>();
}

Continent::Continent(string name)
{
	name_ = &name;
	countries_ = new vector<Country*>();
}


string* Continent::get_name() const
{
	return name_;
}

vector<Country*>* Continent::get_countries() const
{
	return countries_;
}

void Continent::set_name(string* name)
{
	name_ = name;
}

void Continent::validate_continent(Country* country, bool* visited_countries) const
{
	visited_countries[get_country_index(country)] = true;
	for (auto const& c : *country->get_adjacent_countries())
	{
		//In this case, validate that the country is also within the continent.
		if (c->get_continent()->get_name() == name_ && !visited_countries[get_country_index(c)])
		{
			validate_continent(c, visited_countries);
		}
	}
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

Country::Country(): 
	name_(nullptr), 
	continent_(nullptr), 
	player_(nullptr), 
	nb_armies_(nullptr)
{
	adjacent_countries_ = new vector<Country*>();
}

Country::Country(string name)
{
	name_ = new string(name);
	adjacent_countries_ = new vector<Country*>();
}


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