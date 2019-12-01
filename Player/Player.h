#pragma once

#include <vector>
#include <string>
#include <GameObservers/GameObservers.h>
#include "Map/Map.h"
#include "Cards/Cards.h"
#include "Dice/Dice.h"
#include "PlayerStrategies/PlayerStrategies.h"

class Strategy;


class Player : public PhaseObservable {
public:
    explicit Player(Deck* deck);

    Player(std::string name, Deck* deck);

    Player(std::string player_name, Deck* deck, Strategy* newStrategy);

    ~Player();

    [[nodiscard]] std::string get_player_name() const { return *this->player_name; };

    [[nodiscard]] vector<Country*>* get_player_owned_countries() const { return this->owned_countries; };

    void add_country(Country* new_country);

    void remove_country(const std::string& removed_country);

    [[nodiscard]] vector<Continent*>* get_player_owned_continents() const { return this->owned_continents; };

    void add_continent(Continent* new_continent);

    void remove_continent(const std::string& removed_continent);

    [[nodiscard]] Hand* get_hand() const { return this->hand; };

    [[nodiscard]] Strategy* get_strategy() const { return this->strategy; };

    [[nodiscard]] vector<Dice*>* get_dice_container() const { return this->dice_container; };

    void reinforce(); //handles number of troops added each turn

    void attack();

    void fortify(); //handles the movement of troops into owned territories

    void setStrategy(Strategy* newStrategy);


private:
    string* player_name;
    vector<Country*>* owned_countries;
    vector<Continent*>* owned_continents;
    Hand* hand;
    Deck* deck;
    Strategy* strategy;
    vector<Dice*>* dice_container;

    int* print_and_return_index_country_selected();
};
