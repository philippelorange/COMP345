#include<iostream>

#include "PlayerStrategies.h"
#include "Player/Player.h"

void AggressiveStrategy::place_army(vector<Country*>* countries) {
    //Find strongest country, and place the army there
    Country* strongestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (strongestCountry->get_nb_armies() < countries->at(i)->get_nb_armies()) {
            strongestCountry = countries->at(i);
        }
    }

    strongestCountry->add_army();
}

Country* AggressiveStrategy::get_country_to_reinforce(vector<Country*>* countries) {
    //Find strongest country
    Country* strongestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (strongestCountry->get_nb_armies() < countries->at(i)->get_nb_armies()) {
            strongestCountry = countries->at(i);
        }
    }

    return strongestCountry;
}

bool AggressiveStrategy::should_fortify() { return true; }

Country* AggressiveStrategy::get_country_to_fortify(vector<Country*>* countries) {
    //get countries that have at least one neighbor with two armies or more
    Country* country_to_fortify = countries->at(0);

    for (auto& c : *countries) {
        if (c->get_nb_armies() > country_to_fortify->get_nb_armies()) {
            country_to_fortify = c;
        }
    }

    return country_to_fortify;
}

Country* AggressiveStrategy::get_fortification_source(Country* destination) {
    Country* max_country = destination->get_adjacent_countries()->at(0);
    Player* destination_owner = destination->get_player();
    for (auto& c : *destination->get_adjacent_countries()) {
        if (c->get_nb_armies() > max_country->get_nb_armies() && c->get_player() == destination_owner) {
            max_country = c;
        }
    }

    return max_country;
}

int AggressiveStrategy::get_fortification_armies(Country* source) {
    return source->get_nb_armies() - 1;
}

bool AggressiveStrategy::should_attack() { return true; }

Country* AggressiveStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    Country* max_country = countries->at(0);

    for (auto& c : *countries) {
        if (c->get_nb_armies() > max_country->get_nb_armies()) {
            max_country = c;
        }
    }

    return max_country;
}

Country* AggressiveStrategy::get_country_to_attack(vector<Country*>* countries) {
    //finds weakest target country
    Country* attack_target = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (attack_target->get_nb_armies() < countries->at(i)->get_nb_armies()) {
            attack_target = countries->at(i);
        }
    }

    return attack_target;
}

int AggressiveStrategy::get_attack_dice(int max_dice) {
    return max_dice;
}

int AggressiveStrategy::get_defend_dice(int max_dice) {
    return max_dice;
}

int AggressiveStrategy::get_armies_to_move(int max) {
    return 1; //always move only a single army, to keep the strongest one stacked
}

string AggressiveStrategy::get_name() {
    return "Aggressive";
}

bool AggressiveStrategy::should_cheat() {
    return false;
}

void AggressiveStrategy::cheat_attack(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void AggressiveStrategy::cheat_reinforce(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void AggressiveStrategy::cheat_fortify(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void BenevolentStrategy::place_army(vector<Country*>* countries) {
    //Place army on the weakest army to date, to create an even distribution
    Country* weakestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (weakestCountry->get_nb_armies() > countries->at(i)->get_nb_armies()) {
            weakestCountry = countries->at(i);
        }
    }

    weakestCountry->add_army();
}

Country* BenevolentStrategy::get_country_to_reinforce(vector<Country*>* countries) {
    //Find weakest country
    Country* weakestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (weakestCountry->get_nb_armies() > countries->at(i)->get_nb_armies()) {
            weakestCountry = countries->at(i);
        }
    }

    return weakestCountry;
}

bool BenevolentStrategy::should_fortify() { return true; }

Country* BenevolentStrategy::get_country_to_fortify(vector<Country*>* countries) {
    int min_armies = countries->at(0)->get_nb_armies();
    Country* country_to_fortify = countries->at(0);

    for (auto& c : *countries) {
        if (c->get_nb_armies() < min_armies) {
            country_to_fortify = c;
        }
    }

    return country_to_fortify;
}

Country* BenevolentStrategy::get_fortification_source(Country* destination) {
    Country* min_country = destination->get_adjacent_countries()->at(0);
    Player* destination_owner = destination->get_player();
    for (auto& c : *destination->get_adjacent_countries()) {
        if (c->get_nb_armies() < min_country->get_nb_armies() && c->get_player() == destination_owner) {
            min_country = c;
        }
    }

    return min_country;
}

int BenevolentStrategy::get_fortification_armies(Country* source) {
    return source->get_nb_armies() - 1;
}

bool BenevolentStrategy::should_attack() { return false; }

Country* BenevolentStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    return nullptr; //BenevolentStrategy should never attack in the first place
}

Country* BenevolentStrategy::get_country_to_attack(vector<Country*>* countries) {
    return nullptr; //BenevolentStrategy should never attack in the first place
}

int BenevolentStrategy::get_attack_dice(int max_dice) {
    return max_dice; //Won't get called
}

int BenevolentStrategy::get_defend_dice(int max_dice) {
    return max_dice;
}

int BenevolentStrategy::get_armies_to_move(int max) {
    return 1; //wont get called
}

string BenevolentStrategy::get_name() {
    return "Benevolent";
}

bool BenevolentStrategy::should_cheat() {
    return false;
}

void BenevolentStrategy::cheat_attack(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void BenevolentStrategy::cheat_reinforce(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void BenevolentStrategy::cheat_fortify(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void HumanStrategy::place_army(vector<Country*>* countries) {
    int selection = -1;
    while (selection < 1 || selection > countries->size()) {
        cout << "\t, Please place an army." << endl;
        for (int k = 0; k < countries->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << countries->at(k)->get_name()
                 << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > countries->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    countries->at(selection - 1)->add_army();
}

Country* HumanStrategy::get_country_to_reinforce(vector<Country*>* countries) {
    int selection = 0;
    while (selection < 1 || selection > countries->size()) {
        cout << "\t" << "Please place an army amongst the following options:" << endl;
        for (int k = 0; k < countries->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << countries->at(k)->get_name()
                 << endl;
        }
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > countries->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }
    return countries->at(selection - 1);
}

bool HumanStrategy::should_fortify() {
    string answer;

    while (answer != "yes" && answer != "no") {
        cout << "Would you like to fortify a country? (yes/no)" << endl;
        cin >> answer;
    }

    return answer == "yes";
}

Country* HumanStrategy::get_country_to_fortify(vector<Country*>* countries) {
    int selection = 0;

    while (selection < 1 || selection > countries->size()) {
        cout << "Please select a country to fortify: " << endl;
        for (int k = 0; k < countries->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << countries->at(k)->get_name() << endl;
        }
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > countries->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }
    return countries->at(selection - 1);
}

Country* HumanStrategy::get_fortification_source(Country* destination) {
    int selection = 0;
    Player* destination_owner = destination->get_player();
    auto* possible_sources = new vector<Country*>();
    for (auto& c : *destination->get_adjacent_countries()) {
        if (c->get_nb_armies() > 1 && c->get_player() == destination_owner) {
            possible_sources->push_back(c);
        }
    }

    while (selection < 1 || selection > possible_sources->size()) {
        cout << "Please select the country to move the armies from: " << endl;
        for (int k = 0; k < possible_sources->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << possible_sources->at(k)->get_name() << endl;
        }
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > possible_sources->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }
    return possible_sources->at(selection - 1);
}

int HumanStrategy::get_fortification_armies(Country* source) {
    int selection = 0;

    while (selection < 1 || selection > source->get_nb_armies() - 1) {
        cout << "Please select how many armies you would like to transfer: " << endl;
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > source->get_nb_armies() - 1) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }
    return selection;
}

bool HumanStrategy::should_attack() {
    string answer;
    do {
        cout << "Would you like to perform an attack? (yes/no)" << endl;
        cin >> answer;
    } while (!(answer == "yes" || answer == "no"));

    return answer == "yes";
}

Country* HumanStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    //Prints the list of countries that the player can attack from and prompts the user to chose among them.
    int selection = -1;
    while (selection < 1 || selection > countries->size()) {
        cout << "Please select a country to attack from among the following countries:" << endl;
        for (int k = 0; k < countries->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << countries->at(k)->get_name() << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > countries->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    return countries->at(selection - 1);
}

Country* HumanStrategy::get_country_to_attack(vector<Country*>* countries) {
    int selection = -1;

    while (selection < 1 || selection > countries->size()) {
        cout << "Please select a country to attack amongst the following countries:" << endl;
        for (int k = 0; k < countries->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << countries->at(k)->get_name() << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > countries->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    return countries->at(selection - 1);
}

int HumanStrategy::get_attack_dice(int max_dice) {
    int input = -1;
    do {
        cout << "How many dices do you want to attack with? (between 1 and " << max_dice << ")" << endl;
        cin >> input;
    } while (!(input > 0 && input <= max_dice));

    return input;
}

int HumanStrategy::get_defend_dice(int max_dice) {
    int input = -1;
    do {
        cout << "How many dices do you want to defend with? (between 1 and " << max_dice << ")" << endl;
        cin >> input;
    } while (!(input > 0 && input <= max_dice));

    return input;
}

int HumanStrategy::get_armies_to_move(int max) {
    cout << "Select the number of troops you want to move:" << endl;
    int armies_moved_to_conquered_country;
    do {
        cin >> armies_moved_to_conquered_country;
    } while (!(armies_moved_to_conquered_country > 0 &&
               armies_moved_to_conquered_country <= max));

    return armies_moved_to_conquered_country;
}

string HumanStrategy::get_name() {
    return "Human";
}

bool HumanStrategy::should_cheat() {
    return false;
}

void HumanStrategy::cheat_attack(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void HumanStrategy::cheat_reinforce(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void HumanStrategy::cheat_fortify(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void RandomStrategy::place_army(vector<Country*>* countries) {
    //Find random country, and place the army there
    int random = rand() % countries->size() + 0;
    Country* randomCountry = countries->at(random);

    randomCountry->add_army();
}

Country* RandomStrategy::get_country_to_reinforce(vector<Country*>* countries) {
    //Find random country
    int random = rand() % countries->size() + 0;
    Country* randomCountry = countries->at(random);

    return randomCountry;
}

bool RandomStrategy::should_fortify() {
    return true;
}

Country* RandomStrategy::get_country_to_fortify(vector<Country*>* countries) {
    //return random country
    int random = rand() % countries->size() + 0;
    Country* country_to_fortify = countries->at(random);

    return country_to_fortify;
}

Country* RandomStrategy::get_fortification_source(Country* destination) {
    int random;
    do {
        random = rand() % destination->get_adjacent_countries()->size() + 0;
    } while (destination->get_adjacent_countries()->at(random)->get_player()->get_player_name() !=
             destination->get_player()->get_player_name()); // random country must be owned by player

    Country* random_country = destination->get_adjacent_countries()->at(random);

    return random_country;
}

int RandomStrategy::get_fortification_armies(Country* source) {
    return source->get_nb_armies() - 1;
}

bool RandomStrategy::should_attack() {
    bool attack = false;
    int random = rand() % 2 + 0;
    attack = random != 0;
    return attack;
}

Country* RandomStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    int random = rand() % countries->size() + 0;
    Country* random_country = countries->at(random);

    return random_country;
}

Country* RandomStrategy::get_country_to_attack(vector<Country*>* countries) {
    //finds weakest target country
    int random = rand() % countries->size() + 0;
    Country* attack_target = countries->at(random);

    return attack_target;
}

int RandomStrategy::get_attack_dice(int max_dice) {
    return rand() % max_dice + 1;
}

int RandomStrategy::get_defend_dice(int max_dice) {
    return rand() % max_dice + 1;
}

int RandomStrategy::get_armies_to_move(int max) {
    return rand() % max + 1;
}

string RandomStrategy::get_name() {
    return "Random";
}

void RandomStrategy::cheat_attack(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void RandomStrategy::cheat_reinforce(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

void RandomStrategy::cheat_fortify(Player* player) {
    cout << this->get_name() << " cannot cheat !" << endl;
}

bool RandomStrategy::should_cheat() {
    return false;
}


void CheaterStrategy::place_army(vector<Country*>* countries) {
    // Will place army on weakest country, best strategy for cheater since he want's to double army on countries
    Country* weakestCountry = countries->at(0);

    for (int i = 1; i < countries->size(); i++) {
        if (weakestCountry->get_nb_armies() > countries->at(i)->get_nb_armies()) {
            weakestCountry = countries->at(i);
        }
    }

    weakestCountry->add_army();
}

Country* CheaterStrategy::get_country_to_reinforce(vector<Country*>* countries) {
    return nullptr;
}

bool CheaterStrategy::should_fortify() {
    return false;
}

Country* CheaterStrategy::get_country_to_fortify(vector<Country*>* countries) {
    return nullptr;
}

Country* CheaterStrategy::get_fortification_source(Country* destination) {
    return nullptr;
}

int CheaterStrategy::get_fortification_armies(Country* source) {
    return 0;
}

bool CheaterStrategy::should_attack() {
    return false;
}

Country* CheaterStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    return nullptr;
}

Country* CheaterStrategy::get_country_to_attack(vector<Country*>* countries) {
    return nullptr;
}

int CheaterStrategy::get_attack_dice(int max_dice) {
    return max_dice;
}

int CheaterStrategy::get_defend_dice(int max_dice) {
    return max_dice;
}

int CheaterStrategy::get_armies_to_move(int max) {
    return 0;
}

string CheaterStrategy::get_name() {
    return "Cheater";
}

bool CheaterStrategy::should_cheat() {
    return true;
}

void CheaterStrategy::cheat_attack(Player* player) {
    for (Country* country: *player->get_player_owned_countries()) {
        for (Country* adj_country : *country->get_adjacent_countries()) {
            if (adj_country->get_player()->get_player_name() != country->get_player()->get_player_name()) {
                cout << adj_country->get_name() << " has been conquered by " << player->get_player_name() << endl;
                adj_country->get_player()->remove_country(adj_country->get_name());
                player->add_country(adj_country);
            }
        }
    }
}

void CheaterStrategy::cheat_reinforce(Player* player) {
    for (Country* country: *player->get_player_owned_countries()) {
        cout << country->get_name() << " has :" << country->get_nb_armies() << " armies" << endl;
        country->set_nb_armies(country->get_nb_armies() * 2);
        cout << country->get_name() << " now has :" << country->get_nb_armies() << " armies" << endl;
    }
}

void CheaterStrategy::cheat_fortify(Player* player) {
    for (Country* country: *player->get_player_owned_countries()) {
        for (Country* adj_country : *country->get_adjacent_countries()) {
            if (adj_country->get_player()->get_player_name() != country->get_player()->get_player_name()) {
                cout << country->get_name() << " has :" << country->get_nb_armies() << " armies" << endl;
                country->set_nb_armies(country->get_nb_armies() * 2);
                cout << country->get_name() << " now has :" << country->get_nb_armies() << " armies" << endl;
                break;
            }
        }
    }
}
