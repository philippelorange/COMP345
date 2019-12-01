#include<iostream>
#include <utility>
#include <vector>
#include "Map/Map.h"
#include "Cards/Cards.h"
#include "Dice/Dice.h"
#include "Player.h"
#include <algorithm>

Player::Player(Deck* deck) {
    player_name = new string("Default");
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    this->deck = deck;
    this->strategy = new HumanStrategy();

}

Player::Player(std::string player_name, Deck* deck) {
    this->player_name = new string(std::move(player_name));
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    this->deck = deck;
    this->strategy = new HumanStrategy();
}

Player::Player(std::string player_name, Deck* deck, Strategy* newStrategy) {
    this->player_name = new string(std::move(player_name));
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    this->deck = deck;
    this->strategy = newStrategy;

}

Player::~Player() {
    delete this->player_name;
    delete this->hand;
    delete this->deck;

    for (Country* owned_country : *this->owned_countries) {
        delete owned_country;
    }
    delete this->owned_countries;

    for (Continent* owned_continent : *owned_continents) {
        delete owned_continent;
    }
    delete this->owned_continents;

    for (Dice* dice : *this->dice_container) {
        delete dice;
    }
    delete this->dice_container;
};

void Player::add_country(Country* new_country) {
    this->owned_countries->push_back(new_country);
    new_country->set_player(this);
}

void Player::remove_country(const std::string& removed_country) {
    for (int i = 0; i < owned_countries->size(); i++) {
        if (removed_country == (*owned_countries)[i]->get_name()) {
            owned_countries->erase(owned_countries->begin() + i);
        }
    }
}

void Player::add_continent(Continent* new_continent) {
    this->owned_continents->push_back(new_continent);
}

void Player::remove_continent(const std::string& removed_continent) {
    for (int i = 0; i < owned_continents->size(); i++) {
        if (removed_continent == (*owned_continents)[i]->get_name()) {
            owned_continents->erase(owned_continents->begin() + i);
        }
    }
}

void Player::reinforce() {
    this->notify(GamePhase::reinforcement);

    if (!strategy->should_cheat()) {
        //Start by finding how many armies we can place
        //#countries owned/3 (rounded down)
        int num_armies = owned_countries->size() / 3;

        //add num of continents owned (continent control value)
        for (auto& owned_continent : *this->owned_continents) {
            num_armies += owned_continent->get_control_value();
        }

        //exchange if more than 4 cards
        if (this->hand->get_hand_cards()->size() > 4) {
            num_armies += this->hand->exchange(this->deck);
        }
        //if armies is less than 3, set to 3
        if (num_armies < 3) {
            num_armies = 3;
        }

        for (int i = 0; i < num_armies; i++) {
            auto* country_to_reinforce = strategy->get_country_to_reinforce(owned_countries);
            country_to_reinforce->add_army();

            cout << *player_name << " has placed an army at " << country_to_reinforce->get_name() << "." << endl;
            cout << country_to_reinforce->get_name() << " has " << country_to_reinforce->get_nb_armies() << " armies"
                 << endl;
        }
    } else {
        strategy->cheat_reinforce(this);
    }
}

void Player::attack() {
    this->notify(GamePhase::attack);

    if (!strategy->should_cheat()) {
        while (strategy->should_attack()) {
            cout << *player_name << " has chosen to attack!" << endl;

            auto* countries_to_attack_from = new vector<Country*>();

            for (auto& c : *owned_countries) {
                if (c->get_nb_armies() >= 2) {
                    for (auto& adj : *c->get_adjacent_countries()) {
                        if (adj->get_nb_armies() > 0 & adj->get_player() != this) {
                            countries_to_attack_from->push_back(c);
                        }
                    }
                }
            }

            if (countries_to_attack_from->empty()) {
                cout << *player_name << " no longer has armies capable of attacking." << endl;
                return;
            }

            auto* country_to_attack_from = strategy->get_country_to_attack_from(countries_to_attack_from);

            auto* valid_targets = new vector<Country*>();
            for (Country* c : *(country_to_attack_from->get_adjacent_countries())) {
                if (c->get_player() != this) {
                    valid_targets->push_back(c);
                }
            }

            Country* country_to_attack = strategy->get_country_to_attack(valid_targets);

            auto* defending_player = country_to_attack->get_player();

            //Instantiating all variables that will be used in the attack process
            bool battle_is_over = false;
            bool attacker_won = false;
            string answer;

            int armies_in_attacking_country = country_to_attack_from->get_nb_armies();
            int max_number_of_dices_attack;
            int number_of_dices_attack;
            int armies_in_defending_country = country_to_attack->get_nb_armies();
            int max_number_of_dices_defense;
            int number_of_dices_defense;

            //Battle loop
            while (!battle_is_over) {

                max_number_of_dices_attack = (armies_in_attacking_country <= 4) ? armies_in_attacking_country - 1 : 3;
                max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;

                number_of_dices_attack = strategy->get_attack_dice(max_number_of_dices_attack);

                number_of_dices_defense = defending_player->get_strategy()->get_defend_dice(
                        max_number_of_dices_defense);

                cout << "** Starting battle **" << endl;

                int number_of_comparisons = min(number_of_dices_attack, number_of_dices_defense);
                vector<int>* attacker_rolls = Dice::sortDsc(number_of_dices_attack);
                vector<int>* defender_rolls = Dice::sortDsc(number_of_dices_defense);

                cout << "Attacker has: " << armies_in_attacking_country << " troops" << endl;
                cout << "Defender has: " << armies_in_defending_country << " troops" << endl;

                cout << "Number of dice comparisons: " << number_of_comparisons << endl;

                for (int i = 0; i < number_of_comparisons; i++) {

                    cout << "Roll #" << (i + 1) << ": Attacker rolled a " << attacker_rolls->at(i)
                         << ", defender rolled a "
                         << defender_rolls->at(i) << endl;
                    if (attacker_rolls->at(i) > defender_rolls->at(i)) {
                        cout << "\tAttacker wins this roll." << endl;
                        armies_in_defending_country--;
                    } else {
                        cout << "\tDefender wins this roll." << endl;
                        armies_in_attacking_country--;
                    }
                }

                delete attacker_rolls;
                delete defender_rolls;

                //Checks for battle end-conditions
                if (armies_in_attacking_country == 1)
                    battle_is_over = true;

                if (armies_in_defending_country == 0) {
                    battle_is_over = true;
                    attacker_won = true;
                }

                cout << "Attacker has: " << armies_in_attacking_country << " troops" << endl;
                cout << "Defender has: " << armies_in_defending_country << " troops" << endl;

            }
            //adjusts number of armies post battle
            country_to_attack_from->set_nb_armies(armies_in_attacking_country);
            country_to_attack->set_nb_armies(armies_in_defending_country);
            if (attacker_won) {
                defending_player->remove_country(country_to_attack->get_name());
                this->add_country(country_to_attack);
                cout << *player_name << " has conquered " << country_to_attack->get_name() << endl;
                int armies_moved_to_conquered_country = strategy->get_armies_to_move(armies_in_attacking_country - 1);
                cout << *player_name << " selected to move " << armies_moved_to_conquered_country << " armies" << endl;

                country_to_attack_from->set_nb_armies(
                        (country_to_attack_from->get_nb_armies()) - armies_moved_to_conquered_country);
                country_to_attack->set_nb_armies(armies_moved_to_conquered_country);
            }

            cout << "*** Battle is over ***" << endl;
        }
    } else {
        strategy->cheat_attack(this);
    }
}

void Player::fortify() {
    this->notify(GamePhase::fortify);
    if (!strategy->should_cheat()) {
        //get countries that have at least one neighbor with two armies or more
        auto* potential_countries = new vector<Country*>();

        for (auto& c : *owned_countries) {
            for (auto& adj : *c->get_adjacent_countries()) {
                if (adj->get_nb_armies() >= 2 && adj->get_player() == this) {
                    potential_countries->push_back(c);
                    break;
                }
            }
        }

        if (potential_countries->empty()) {
            cout << "You do not have enough countries and armies to fortify." << endl;
            return;
        }

        if (!strategy->should_fortify()) return;

        auto* country_to_fortify = strategy->get_country_to_fortify(potential_countries);

        auto* source_country = strategy->get_fortification_source(country_to_fortify);

        cout << "Source country, " << source_country->get_name() << " has :" << source_country->get_nb_armies() <<
             " armies" << endl;

        cout << "Fortified country, " << country_to_fortify->get_name() << " has :" << country_to_fortify->get_nb_armies() <<
             " armies" << endl;

        int armies_to_move = strategy->get_fortification_armies(source_country);

        if (armies_to_move > 0) {
            source_country->set_nb_armies(source_country->get_nb_armies() - armies_to_move);
            country_to_fortify->set_nb_armies(country_to_fortify->get_nb_armies() + armies_to_move);

            cout << *player_name << " has transferred " << armies_to_move << " armies from " <<
                 source_country->get_name() << " to " << country_to_fortify->get_name() << "." << endl;

            cout << country_to_fortify->get_name() << " has " << country_to_fortify->get_nb_armies() << " armies"
                 << endl;
            cout << source_country->get_name() << " has " << source_country->get_nb_armies() << " armies" << endl;
        } else {
            cout << this->get_player_name() << " opted not to fortify his position" << endl;
        }
    } else {
        strategy->cheat_fortify(this);
    }
}

void Player::setStrategy(Strategy* newStrategy) {
    this->strategy = newStrategy;
}

int* Player::print_and_return_index_country_selected() {

    int selection = 0;

    while (selection < 1 || selection > this->get_player_owned_countries()->size()) {
        cout << "\t" << this->get_player_name() << endl;
        for (int k = 0; k < this->get_player_owned_countries()->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << this->get_player_owned_countries()->at(k)->get_name() << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > this->get_player_owned_countries()->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    return new int(selection);
}