#pragma once

#include <string>
#include <vector>

using namespace std;
class Continent;
class Country;
class Player;

class Map
{
public:
	Map();
	Map(vector<Continent*>* continents, vector<Country*>* countries);

	void depth_first_traversal(Country* country, bool* visited_countries) const;
private:
	vector<Continent*>* continents_;
	vector<Country*>* countries_;
	int get_country_index(Country* country) const;

};

class Continent
{
public:
	Continent();
	Continent(string name);

	//getters
	string* get_name() const;
	vector<Country*>* get_countries() const;

	//setters
	void set_name(string* name);

	void validate_continent(Country* country, bool* visited_countries) const;
	void add_country(Country* country) const;

private:
	string* name_;
	vector<Country*>* countries_;

	int get_country_index(Country* country) const;
	bool already_added(Country* country) const;
};

class Country
{
public:
	Country();
	Country(string name);
	//getters
	string get_name() const;
	Continent* get_continent() const;
	Player* get_player() const;
	vector<Country*>* get_adjacent_countries() const;
	int* get_nb_armies() const;

	//setters
	void set_name(string name);
	void set_continent(Continent* continent);
	void set_player(Player* player);
	void set_nb_armies(int* nb_armies);

	void add_adjacent_country(Country* country) const;

private:
	string* name_;
	Continent* continent_;
	Player* player_;
	int* nb_armies_;
	vector<Country*>* adjacent_countries_;

	bool already_added(Country* country) const;
};

