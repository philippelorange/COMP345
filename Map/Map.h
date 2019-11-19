#pragma once

#include <string>
#include <vector>
#include <GameObservers/GameObservers.h>

using namespace std;

class Continent;

class Country;

class Player;

class Map {
public:
    Map();

    Map(vector<Continent*>* continents, vector<Country*>* countries);

    vector<Country*>* get_countries();

    [[nodiscard]] bool validate_connected_graph() const;

    bool validate_continent_singularity();

    bool player_has_been_set();

    void set_name(string s);

    string get_name();

private:
    string* name;
    vector<Continent*>* continents_;
    vector<Country*>* countries_;

    void depth_first_traversal(Country* country, vector<bool>* visited_countries) const;

    int get_country_index(Country* country) const;

};

class Continent {
public:
    Continent(string name, int control_value);

    //getters
    [[nodiscard]] string get_name() const;

    [[nodiscard]] vector<Country*>* get_countries() const;

    [[nodiscard]] int get_control_value() const;

    bool has_country(Country*);

    //setters
    void set_name(string* name);

    void add_country(Country* country) const;

    [[nodiscard]] bool validate_continent() const;

private:
    string* name_;
    vector<Country*>* countries_;
    int* control_value_;

    void depth_first_traversal(Country* country, vector<bool>* visited_countries) const;

    int get_country_index(Country* country) const;

    bool already_added(Country* country) const;
};

class Country : public StatisticObservable {
public:
    Country();

    Country(string name, Continent* continent);

    //getters
    [[nodiscard]] string get_name() const;

    [[nodiscard]] Continent* get_continent() const;

    [[nodiscard]] Player* get_player() const;

    [[nodiscard]] vector<Country*>* get_adjacent_countries() const;

    [[nodiscard]] int get_nb_armies() const;

    //setters
    void set_name(string name);

    void set_continent(Continent* continent);

    void set_player(Player* player);

    void add_army();

    void set_nb_armies(int nb_armies);

    void add_adjacent_country(Country* country) const;

private:
    string* name_;
    Continent* continent_;
    Player* player_;
    int* nb_armies_;
    vector<Country*>* adjacent_countries_;

    bool already_added(Country* country) const;
};

