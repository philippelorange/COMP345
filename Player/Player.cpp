#include<iostream>
#include <vector>
#include "Map/Map.h"
#include "Cards/Cards.h"
#include "Dice/Dice.h"
#include "Player.h"


Player::Player() {
    player_name = "Default";
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    countries_owned_bonus = 3;
    continents_owned_bonus = 0;
}

Player::Player(std::string playername) {
    this->player_name = playername;
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    countries_owned_bonus = 3;
    continents_owned_bonus = 0;
}

Player::~Player() {
    std::cout << "player " << get_player_name() << " was removed from the game" << std::endl;
};

void Player::add_country(Country* new_country) {
    this->owned_countries->push_back(new_country);
    new_country->set_player(this);
}

void Player::remove_country(std::string removed_country) {
    for (int i = 0; i < owned_countries->size(); i++) {
        if (removed_country.compare((*owned_countries)[i]->get_name()) == 0) {
            owned_countries->erase(owned_countries->begin() + i);
        }
    }
}

void Player::add_continent(Continent* new_continent) {
    this->owned_continents->push_back(new_continent);
}

void Player::remove_continent(std::string removed_continent) {
    for (int i = 0; i < owned_continents->size(); i++) {
        if (removed_continent.compare((*owned_continents)[i]->get_name()) == 0) {
            owned_continents->erase(owned_continents->begin() + i);
        }
    }
}

void Player::draw(Deck* deck) {
    deck->draw(this->hand);
}

void Player::reinforce() {
    cout << "the method reinforce has been called by " << this->get_player_name() << std::endl;
}

void Player::attack() {
    cout << "the method attack has been called by " << this->get_player_name() << std::endl;
}

void Player::fortify() {
    cout << "the method fortify has been called by" << this->get_player_name() << std::endl;
    string answer;
    while (true) { // loop until exit
        cout << "What do you wish to do (fortify, exit) ?" << endl;
        cin >> answer;

        if (answer == "fortify") {
            string can_player_fortify = this->can_player_fortify();
            if (can_player_fortify == "You can fortify!") {
                cout << "Here is a list of your owned countries" << endl;

                for (Country* owned_country: *this->owned_countries) {
                    cout << "Country: " << owned_country->get_name() << endl;
                }

                while (true) {
                    cout << "Select the source country" << endl;
                    string source_country;
                    cin >> source_country;
                    cout << "Select the target country" << endl;
                    string target_country;
                    cin >> target_country;
                    bool is_source_country_valid = false;
                    bool is_target_country_valid = false;

                    for (Country* owned_country: *this->owned_countries) {
                        if (source_country == owned_country->get_name()) {
                            is_source_country_valid = true;
                        }

                        if (target_country == owned_country->get_name()) {
                            is_target_country_valid = true;
                        }
                    }

                    if (!is_source_country_valid) {
                        cout << "You do not own this source country!" << endl;
                    }
                    if (!is_target_country_valid) {
                        cout << "You do not own this target country!" << endl;
                    }
                    if (!is_source_country_valid || !is_target_country_valid) {
                        cout << "Data is invalid ! Retry" << endl;
                        continue;
                    }

                    if (source_country == target_country) {
                        cout << "Same name for source country and target country ! Retry" << endl;
                        continue;
                    }

                    if (this->can_player_fortify(source_country, target_country)) {

                        while (true) {
                            int nbr_of_armies_moved;
                            cout << "Enter the number of armies you wish to move" << endl;
                            cin >> nbr_of_armies_moved;
                            auto* source_country_ptr = new Country();
                            auto* target_country_ptr = new Country();
                            for (auto& owned_country : *owned_countries) {
                                if (owned_country->get_name() == source_country) {
                                    source_country_ptr = owned_country;
                                    cout << "Armies in:" << source_country << " : "
                                         << owned_country->get_nb_armies() << endl;
                                }
                                if (owned_country->get_name() == target_country) {
                                    cout << "Armies in:" << target_country << " : "
                                         << owned_country->get_nb_armies() << endl;
                                    target_country_ptr = owned_country;
                                }
                            }

                            if (nbr_of_armies_moved > 0 && nbr_of_armies_moved < source_country_ptr->get_nb_armies()) {
                                source_country_ptr->set_nb_armies(
                                        new int(source_country_ptr->get_nb_armies() - nbr_of_armies_moved));
                                target_country_ptr->set_nb_armies(
                                        new int(target_country_ptr->get_nb_armies() + nbr_of_armies_moved));

                                cout << "There is now " << source_country_ptr->get_nb_armies() << " armies in "
                                     << source_country_ptr->get_name() << endl;

                                cout << "There is now " << target_country_ptr->get_nb_armies() << "armies in "
                                     << target_country_ptr->get_name();
                                break;
                            } else {
                                cout << "Invalid number of armies ! Retry." << endl;
                                cout << "Do you wish to exit ? (yes/no)" << endl;
                                string wish_to_exit;
                                cin >> wish_to_exit;
                                if (wish_to_exit == "yes") {
                                    break;
                                } else {
                                    continue;
                                }
                            }
                        }

                    } else {
                        cout << "These country aren't adjacent to each other! Restart."
                             << endl;
                        continue;
                    }

                    break;
                }
            } else {
                cout << can_player_fortify << endl;
                break;
            }
            break;
        } else if (answer == "exit")
            break;
        else
            cout << "Enter a valid answer" << endl;
    }

}

void Player::update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus) {
    //TODO
}

string Player::can_player_fortify() {
    if (owned_countries->size() < 2) {
        return "You cannot fortify! You own one country...";
    } else {
        for (auto& owned_country_iteration_1 : *owned_countries) { // Get one country
            for (auto& owned_country_iteration_2 : *owned_countries) { // check every other country inside owned countries
                if (owned_country_iteration_1->get_name() == owned_country_iteration_2->get_name()) {
                    continue;
                }
                for (auto& adjacent_country :*owned_country_iteration_2->get_adjacent_countries()) { // check if adjacent country exists
                    if (adjacent_country->get_name() == owned_country_iteration_1->get_name()) {
                        return "You can fortify!";
                    }
                }

            }
        }
    }
    return "You cannot fortify! You do not own a country that is adjacent to another country";
}

bool Player::can_player_fortify(const string& source_country, const string& target_country) {
    for (auto& owned_country : *owned_countries) { // Get one country
        if (owned_country->get_name() == source_country) {
            for (auto& adjacent_country : *owned_country->get_adjacent_countries()) {
                if (adjacent_country->get_name() == target_country) {
                    return true;
                }
            }
        }
    }
    return false;
}