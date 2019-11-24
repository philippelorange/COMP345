#include "PlayerStrategies.h"
#include<iostream>

void AggressiveStrategy::place_army(vector<Country *> *countries) {
    //Find strongest country, and place the army there
    Country* strongestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (strongestCountry->get_nb_armies() < countries->at(i)->get_nb_armies()) {
            strongestCountry = countries->at(i);
        }
    }

    strongestCountry->add_army();
}

Country* AggressiveStrategy::get_country_to_reinforce(vector<Country *>* countries) {
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

Country* AggressiveStrategy::get_country_to_fortify(vector<Country *> *countries) {
    //get countries that have at least one neighbor with two armies or more
    Country *country_to_fortify = countries->at(0);

    for(auto& c : *countries) {
        if(c->get_nb_armies() > country_to_fortify->get_nb_armies()) {
            country_to_fortify = c;
        }
    }

    return country_to_fortify;
}

Country* AggressiveStrategy::get_fortification_source(Country *destination) {
    Country* max_country = destination->get_adjacent_countries()->at(0);

    for(auto& c : *destination->get_adjacent_countries()) {
        if(c->get_nb_armies() > max_country->get_nb_armies()) {
            max_country = c;
        }
    }

    return max_country;
}

int AggressiveStrategy::get_fortification_armies(Country *source) {
    return source->get_nb_armies()-1;
}

bool AggressiveStrategy::should_attack() { return true; }

Country* AggressiveStrategy::get_country_to_attack_from(vector<Country*>* countries) {
    Country* max_country = countries->at(0);

    for(auto& c : *countries) {
        if(c->get_nb_armies() > max_country->get_nb_armies()) {
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

void BenevolentStrategy::place_army(vector<Country *> *countries) {
    //Place army on the weakest army to date, to create an even distribution
    Country* weakestCountry = countries->at(0);
    for (int i = 1; i < countries->size(); i++) {
        if (weakestCountry->get_nb_armies() > countries->at(i)->get_nb_armies()) {
            weakestCountry = countries->at(i);
        }
    }

    weakestCountry->add_army();
}

Country* BenevolentStrategy::get_country_to_reinforce(vector<Country *> *countries) {
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

Country* BenevolentStrategy::get_country_to_fortify(vector<Country *> *countries) {
    int min_armies = countries->at(0)->get_nb_armies();
    Country *country_to_fortify = countries->at(0);

    for(auto& c : *countries) {
        if(c->get_nb_armies() < min_armies) {
            country_to_fortify = c;
        }
    }

    return country_to_fortify;
}

Country* BenevolentStrategy::get_fortification_source(Country *destination) {
    Country* min_country = destination->get_adjacent_countries()->at(0);

    for(auto& c : *destination->get_adjacent_countries()) {
        if(c->get_nb_armies() < min_country->get_nb_armies()) {
            min_country = c;
        }
    }

    return min_country;
}

int BenevolentStrategy::get_fortification_armies(Country *source) {
    return source->get_nb_armies()-1;
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

void HumanStrategy::place_army(vector<Country *> *countries) {
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

Country* HumanStrategy::get_country_to_reinforce(vector<Country *> *countries) {
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

    while(answer != "yes" && answer != "no") {
        cout << "Would you like to fortify a country? (yes/no)" << endl;
        cin >> answer;
    }

    return answer == "yes";
}

Country* HumanStrategy::get_country_to_fortify(vector<Country *> *countries) {
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

Country* HumanStrategy::get_fortification_source(Country *destination) {
    int selection = 0;

    auto* possible_sources = new vector<Country*>();
    for(auto& c : *destination->get_adjacent_countries()) {
        if(c->get_nb_armies() > 1) {
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

int HumanStrategy::get_fortification_armies(Country *source) {
    int selection = 0;

    while (selection < 1 || selection > source->get_nb_armies()-1) {
        cout << "Please select how many armies you would like to transfer: " << endl;
        cin >> selection;
        if (cin.fail() || selection < 1 || selection > source->get_nb_armies()-1) {
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

Country* HumanStrategy::get_country_to_attack_from(vector<Country *> *countries) {
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

Country* HumanStrategy::get_country_to_attack(vector<Country *> *countries) {
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