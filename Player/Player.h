#pragma once

#include <vector>
#include <string>
#include "Map/Map.h"
#include "Cards/Cards.h"
#include "Dice/Dice.h"

class Player {
public:
    Player();

    Player(std::string name);

    ~Player();

    std::string get_player_name() const { return this->player_name; };

    vector<Country*>* get_player_owned_countries() const { return this->owned_countries; };

    void add_country(Country* new_country);

    void remove_country(std::string removed_country);

    vector<Continent*>* get_player_owned_continents() const { return this->owned_continents; };

    void add_continent(Continent* new_continent);

    void remove_continent(std::string removed_continent);

    Hand* get_hand() const { return this->hand; };

    void draw(Deck* deck);

    vector<Dice*>* get_dice_container() const { return this->dice_container; };

    void reinforce(); //handles number of troups added each turn
    void attack();

    void fortify(); //handles the movement of troops into owned territories
    void update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus);

private:
    std::string player_name;
    vector<Country*>* owned_countries;
    vector<Continent*>* owned_continents;
    Hand* hand;
    vector<Dice*>* dice_container;
    int countries_owned_bonus;
    int continents_owned_bonus;
};
