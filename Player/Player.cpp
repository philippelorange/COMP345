#include<iostream>
#include <vector>
#include "Map/Map.h"
#include "Cards/Cards.h"
#include "Dice/Dice.h"
#include "Player.h"
#include <algorithm>


Player::Player() {
    player_name = "Default";
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    countries_owned_bonus = 3;
    continents_owned_bonus = 0;
}

Player::Player(std::string playername, Deck* deck) {
    this->player_name = playername;
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    this->deck = deck;
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
    //#countries owned/3 (rounded down)
    int* num_armies = new int(this->get_player_owned_countries()->size() / 3);

    //add num of continents owned (continent control value)
    /*for (int i = 0; i < this->owned_continents->size(); i++){
        *num_armies += this->owned_continents.get_control_value();
    }*/
    *num_armies += this->owned_continents->size();

    for (int i = 0; i < 5; i++) {
        this->deck->draw(this->hand);
    }

    //exchange if more than 4 cards
    if (this->hand->get_hand_cards()->size() > 4) {
        this->hand->exchange(this->deck);
    }
    //if armies is less than 3, set to 3
    if (*num_armies < 3) {
        *num_armies = 3;
    }

    //place armies at desired countries
    for (int i = 0; i < *num_armies; i++) {
        int selection = -1;
        while (selection < 1 || selection > this->get_player_owned_countries()->size()) {
            cout << "\t" << this->get_player_name() << ", please place an army. You have " << (*num_armies - i)
                 << " left" << endl;
            for (int k = 0; k < this->get_player_owned_countries()->size(); k++) {
                cout << "\t \t (" << (k + 1) << ") " << this->get_player_owned_countries()->at(k)->get_name() << endl;
            }

            cin >> selection;
            if (cin.fail() || selection < 1 || selection > this->get_player_owned_countries()->size()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input." << endl;
            }
            this->get_player_owned_countries()->at(selection - 1)->add_army();
        }
    }

    for (auto& owned_country : *owned_countries) {
        cout << owned_country->get_name() << " has " << owned_country->get_nb_armies() << " armies" << endl;
    }
}

void Player::attack() {
   
	Country* attack_source = nullptr;
	Country* attack_target = nullptr;
	auto* valid_sources = new vector<Country*>();
	auto* valid_target = new vector<Country*>();
	Player* defending_player;

    for (auto& c: *owned_countries) {
        if (c->get_nb_armies() > 1) {
            valid_sources->push_back(c);
        }
    }
	//Checks that the player has countries from which to initiate an attack
    if(valid_sources->empty()) {
        cout << "You do not have any countries capable of performing an attack at this time." << endl;
        return;
    }
	//Prints the list of countries that the player can attack from and prompts the user to chose among them.
    int selection = -1;
    while(selection < 1 || selection > valid_sources->size()) {
        cout <<this->get_player_name()<<", please select a country to attack from among the following countries:" << endl;
        for (int k = 0; k < valid_sources->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << valid_sources->at(k)->get_name() << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > valid_sources->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    attack_source = valid_sources->at(selection-1);

    selection = -1;

	//get the target of the attack
	//Prints the list of countries that the player can attack and prompts the user to chose among them. Checks for countries adjacent to source that are owned by other players.
    for (Country* c :*(attack_source->get_adjacent_countries())) {
        if (c->get_player() != this) {
            valid_target->push_back(c);
        }
    }

    while(selection < 1 || selection > valid_target->size()) {
        cout << player_name << ", please select a country to attack amongst the following countries:" <<endl;
        for (int k = 0; k < valid_target->size(); k++) {
            cout << "\t \t (" << (k + 1) << ") " << valid_target->at(k)->get_name() << endl;
        }

        cin >> selection;
        if (cin.fail() || selection < 1 || selection > valid_target->size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    attack_target = valid_target->at(selection-1);

    defending_player = attack_target->get_player();
	
	//Instantiating all variables that will be used in the attack process
	bool player_wishes_to_attack = true;
	bool battle_is_over = false;
	bool attacker_won = false;
	string answer;

	int armies_in_attacking_country = attack_source->get_nb_armies();
	int max_number_of_dices_attack;
	int number_of_dices_attack;
	int armies_in_defending_country = attack_target->get_nb_armies();
	int max_number_of_dices_defense;
	int number_of_dices_defense;

	//Battle loop
	while (player_wishes_to_attack && (!battle_is_over)) {

	    max_number_of_dices_attack = (armies_in_attacking_country<=4)? armies_in_attacking_country -1 : 3;
		max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;

		do {
			cout << "How many dices do you want to attack with? (between 1 and " << max_number_of_dices_attack << ")" << endl;
			cin >> number_of_dices_attack;
		} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
		do {
			cout << "How many dices does " << defending_player->get_player_name()<<" want to to defend with? (between 1 and " << max_number_of_dices_defense << ")" << endl;
			cin >> number_of_dices_defense;
		} while (!(number_of_dices_defense > 0 && number_of_dices_defense <= max_number_of_dices_defense));

		
		//Here goes the logic for battle, pairwise comparison of dices
		
		
		cout << "** Starting battle **" << endl;

		int number_of_comparisons = min(number_of_dices_attack, number_of_dices_defense);
		vector<int>* attacker_rolls = Dice::sortDsc(number_of_dices_attack);
        vector<int>* defender_rolls = Dice::sortDsc(number_of_dices_defense);

		cout<<"Attacker has: "<< armies_in_attacking_country <<" troups"<<endl;
		cout <<"Defender has: " << armies_in_defending_country << " troups" << endl;

		cout << "Number of dice comparisons: " << number_of_comparisons << endl;

		for (int i = 0; i < number_of_comparisons; i++) {

		    cout << "Roll #" << (i+1) << ": Attacker rolled a " << attacker_rolls->at(i) << ", defender rolled a " << defender_rolls->at(i) << endl;
			if (attacker_rolls->at(i) > defender_rolls->at(i)) {
                cout << "\tAttacker wins this roll." << endl;
                armies_in_defending_country--;
            }
			else {
                cout << "\tDefender wins this roll." << endl;
                armies_in_attacking_country--;
            }
		}

		delete attacker_rolls;
		delete defender_rolls;

		//Checks for battle end-contions
		if (armies_in_attacking_country == 1)
			battle_is_over = true;
		
		if (armies_in_defending_country == 0) {
			battle_is_over = true;
			attacker_won = true;
		}

		cout << "Attacker has: " << armies_in_attacking_country << " troups" << endl;
		cout << "Defender has: " << armies_in_defending_country << " troups" << endl;
		
		if (!battle_is_over) {
			do {
				cout << player_name << " do you wish to continue the attack? (y/n)" << endl;
				cin >> answer;
			} while (!(answer == "y" || answer == "n"));
			if (answer == "y")
				player_wishes_to_attack = true;
			else
				player_wishes_to_attack = false;
		}
	}
	//adjusts number of armies post battle
	attack_source->set_nb_armies(armies_in_attacking_country);
	attack_target->set_nb_armies(armies_in_defending_country);
	if (attacker_won) {
		this->add_country(attack_target);
		defending_player->remove_country(attack_target->get_name());
		cout<<player_name<<" has conquered "<< attack_target->get_name()<<endl;
		cout<< player_name <<" , select the number of troops you want to move to "<<attack_target->get_name()<<"(between 1 and "<< (armies_in_attacking_country-1) <<")"<<endl;
		int armies_moved_to_conquered_country;
		do {
			cin >> armies_moved_to_conquered_country;
		} while (!(armies_moved_to_conquered_country > 0 && armies_moved_to_conquered_country < armies_in_attacking_country));
		attack_source->set_nb_armies((attack_source->get_nb_armies()) - armies_moved_to_conquered_country);
		attack_target->set_nb_armies(armies_moved_to_conquered_country);
	}

    cout << "*** Battle is over ***" << endl;
}

void Player::fortify() {
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

                    string can_player_fortify_source_and_target = this->can_player_fortify(source_country,
                                                                                           target_country);

                    if (can_player_fortify_source_and_target == "Can fortify") {

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
                                source_country_ptr->set_nb_armies(source_country_ptr->get_nb_armies() - nbr_of_armies_moved);
                                target_country_ptr->set_nb_armies(target_country_ptr->get_nb_armies() + nbr_of_armies_moved);

                                cout << "There is now " << source_country_ptr->get_nb_armies() << " armies in "
                                     << source_country_ptr->get_name() << endl;

                                cout << "There is now " << target_country_ptr->get_nb_armies() << " armies in "
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
                        cout << can_player_fortify_source_and_target
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

string Player::can_player_fortify(const string& source_country, const string& target_country) {
    for (auto& owned_country : *owned_countries) { // Get one country
        if (owned_country->get_name() == source_country) {
            if (owned_country->get_nb_armies() == 1) {
                return "Cannot fortify! Source country only has 1 army";
            }
            for (auto& adjacent_country : *owned_country->get_adjacent_countries()) {
                if (adjacent_country->get_name() == target_country) {
                    return "Can fortify";
                }
            }
        }
    }
    return "Cannot fortify! Countries aren't adjacent";
}