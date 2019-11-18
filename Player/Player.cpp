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
	this->strategy = new Human();

}

Player::Player(std::string player_name, Deck* deck) {
    this->player_name = new string(std::move(player_name));
    this->hand = new Hand();
    this->owned_countries = new vector<Country*>;
    this->owned_continents = new vector<Continent*>;
    this->dice_container = new vector<Dice*>;
    this->deck = deck;
	this->strategy = new Human();
}

Player::Player(std::string player_name, Deck* deck, Strategy *newStrategy) {
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
	int* type = strategy->execute_strategy();
	if (*type == 1) {
		//#countries owned/3 (rounded down)
		int* num_armies = new int(this->get_player_owned_countries()->size() / 3);

		//add num of continents owned (continent control value)
		for (auto& owned_continent : *this->owned_continents) {
			*num_armies += owned_continent->get_control_value();
		}

		//exchange if more than 4 cards
		if (this->hand->get_hand_cards()->size() > 4) {
			*num_armies += this->hand->exchange(this->deck);
		}
		//if armies is less than 3, set to 3
		if (*num_armies < 3) {
			*num_armies = 3;
		}

		//Find strongest country
		Country* strongestCountry = this->get_player_owned_countries()->at(0);
		for (int i = 1; i < this->get_player_owned_countries()->size(); i++) {
			if (strongestCountry->get_nb_armies() < this->get_player_owned_countries()->at(i)->get_nb_armies())
			{
				strongestCountry = this->get_player_owned_countries()->at(i);
			}
		}

		//place armies at strongest country
		for (int i = 0; i < *num_armies; i++) {
			strongestCountry->add_army();
		}

		cout << this->get_player_name() << " has placed " << *num_armies << " armies at " << strongestCountry->get_name() << endl;
		for (auto& owned_country : *owned_countries) {
			cout << owned_country->get_name() << " has " << owned_country->get_nb_armies() << " armies" << endl;
		}
	}


	else if (*type == 2) {
		//#countries owned/3 (rounded down)
		int* num_armies = new int(this->get_player_owned_countries()->size() / 3);

		//add num of continents owned (continent control value)
		for (auto& owned_continent : *this->owned_continents) {
			*num_armies += owned_continent->get_control_value();
		}

		//exchange if more than 4 cards
		if (this->hand->get_hand_cards()->size() > 4) {
			*num_armies += this->hand->exchange(this->deck);
		}
		//if armies is less than 3, set to 3
		if (*num_armies < 3) {
			*num_armies = 3;
		}

		//Find strongest country
		Country* weakestCountry = this->get_player_owned_countries()->at(0);
		for (int i = 1; i < this->get_player_owned_countries()->size(); i++) {
			if (weakestCountry->get_nb_armies() > this->get_player_owned_countries()->at(i)->get_nb_armies())
			{
				weakestCountry = this->get_player_owned_countries()->at(i);
			}
		}

		//place armies at strongest country
		for (int i = 0; i < *num_armies; i++) {
			weakestCountry->add_army();
		}

		cout << this->get_player_name() << " has placed " << *num_armies << " armies at " << weakestCountry->get_name() << endl;
		for (auto& owned_country : *owned_countries) {
			cout << owned_country->get_name() << " has " << owned_country->get_nb_armies() << " armies" << endl;
	}


	}
	else if (*type == 3) {
		//#countries owned/3 (rounded down)
		int* num_armies = new int(this->get_player_owned_countries()->size() / 3);

		//add num of continents owned (continent control value)
		for (auto& owned_continent : *this->owned_continents) {
			*num_armies += owned_continent->get_control_value();
		}

		//exchange if more than 4 cards
		if (this->hand->get_hand_cards()->size() > 4) {
			*num_armies += this->hand->exchange(this->deck);
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
}

void Player::attack() {
    this->notify(GamePhase::attack);
	int* type = strategy->execute_strategy();
	if (*type == 1) {
		Country* attack_source = nullptr;
		Country* attack_target = nullptr;
		auto* valid_sources = new vector<Country*>();
		auto* valid_target = new vector<Country*>();
		Player* defending_player;

		for (auto& c : *owned_countries) {
			if (c->get_nb_armies() > 1) {
				valid_sources->push_back(c);
			}
		}
    

		if (valid_sources->empty()) {
			return;
		}
		//finds strongest source country
		attack_source = valid_sources->at(0);
		for (int i = 1; i < valid_sources->size(); i++) {
			if (attack_source->get_nb_armies() < valid_sources->at(i)->get_nb_armies())
			{
				attack_source = valid_sources->at(i);
			}
		}

		for (Country* c : *(attack_source->get_adjacent_countries())) {
			if (c->get_player() != this) {
				valid_target->push_back(c);
			}
		}

		//finds weakest target country
		attack_target = valid_target->at(0);
		for (int i = 1; i < valid_target->size(); i++) {
			if (attack_target->get_nb_armies() < valid_target->at(i)->get_nb_armies())
			{
				attack_target = valid_target->at(i);
			}
		}
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

			max_number_of_dices_attack = (armies_in_attacking_country <= 4) ? armies_in_attacking_country - 1 : 3;
			max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;

			if (*defending_player->strategy->execute_strategy() == 3) {
				do {
					number_of_dices_attack = max_number_of_dices_attack;
				} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
				do {
					cout << "How many dices does " << defending_player->get_player_name()
						<< " want to to defend with? (between 1 and " << max_number_of_dices_defense << ")" << endl;
					cin >> number_of_dices_defense;
				} while (!(number_of_dices_defense > 0 && number_of_dices_defense <= max_number_of_dices_defense));
			}
			else {
					number_of_dices_attack = max_number_of_dices_attack;
					number_of_dices_defense = max_number_of_dices_defense;
			}

			//Here goes the logic for battle, pairwise comparison of dices


			cout << "** Starting battle **" << endl;

			int number_of_comparisons = min(number_of_dices_attack, number_of_dices_defense);
			vector<int>* attacker_rolls = Dice::sortDsc(number_of_dices_attack);
			vector<int>* defender_rolls = Dice::sortDsc(number_of_dices_defense);

			cout << "Attacker has: " << armies_in_attacking_country << " troups" << endl;
			cout << "Defender has: " << armies_in_defending_country << " troups" << endl;

			cout << "Number of dice comparisons: " << number_of_comparisons << endl;

			for (int i = 0; i < number_of_comparisons; i++) {

				cout << "Roll #" << (i + 1) << ": Attacker rolled a " << attacker_rolls->at(i) << ", defender rolled a "
					<< defender_rolls->at(i) << endl;
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
				player_wishes_to_attack = answer == "y";
			}
		}
		//adjusts number of armies post battle
		attack_source->set_nb_armies(armies_in_attacking_country);
		attack_target->set_nb_armies(armies_in_defending_country);
		if (attacker_won) {
			this->add_country(attack_target);
			defending_player->remove_country(attack_target->get_name());
			cout << player_name << " has conquered " << attack_target->get_name() << endl;
			cout << player_name << " , select the number of troops you want to move to " << attack_target->get_name()
				<< "(between 1 and " << (armies_in_attacking_country - 1) << ")" << endl;
			int armies_moved_to_conquered_country;
			do {
				cin >> armies_moved_to_conquered_country;
			} while (!(armies_moved_to_conquered_country > 0 &&
				armies_moved_to_conquered_country < armies_in_attacking_country));
			attack_source->set_nb_armies((attack_source->get_nb_armies()) - armies_moved_to_conquered_country);
			attack_target->set_nb_armies(armies_moved_to_conquered_country);
		}

		cout << "*** Battle is over ***" << endl;
	}
	else if (*type == 2) {
		cout << this->get_player_name() << "has declined to attack";
		return;
	}
	else if (*type == 3) {
		Country* attack_source = nullptr;
		Country* attack_target = nullptr;
		auto* valid_sources = new vector<Country*>();
		auto* valid_target = new vector<Country*>();
		Player* defending_player;

		for (auto& c : *owned_countries) {
			if (c->get_nb_armies() > 1) {
				valid_sources->push_back(c);
			}
		}
		//Checks that the player has countries from which to initiate an attack
		if (valid_sources->empty()) {
			cout << "You do not have any countries capable of performing an attack at this time." << endl;
			return;
		}
		//Prints the list of countries that the player can attack from and prompts the user to chose among them.
		int selection = -1;
		while (selection < 1 || selection > valid_sources->size()) {
			cout << this->get_player_name() << ", please select a country to attack from among the following countries:"
				<< endl;
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

		attack_source = valid_sources->at(selection - 1);

		selection = -1;

		//get the target of the attack
		//Prints the list of countries that the player can attack and prompts the user to chose among them. Checks for countries adjacent to source that are owned by other players.
		for (Country* c : *(attack_source->get_adjacent_countries())) {
			if (c->get_player() != this) {
				valid_target->push_back(c);
			}
		}

		while (selection < 1 || selection > valid_target->size()) {
			cout << player_name << ", please select a country to attack amongst the following countries:" << endl;
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

		attack_target = valid_target->at(selection - 1);

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

			max_number_of_dices_attack = (armies_in_attacking_country <= 4) ? armies_in_attacking_country - 1 : 3;
			max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;
			if (*defending_player->strategy->execute_strategy() == 3) {
				do {
					cout << "How many dices do you want to attack with? (between 1 and " << max_number_of_dices_attack << ")"
						<< endl;
					cin >> number_of_dices_attack;
				} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
				do {
					cout << "How many dices does " << defending_player->get_player_name()
						<< " want to to defend with? (between 1 and " << max_number_of_dices_defense << ")" << endl;
					cin >> number_of_dices_defense;
				} while (!(number_of_dices_defense > 0 && number_of_dices_defense <= max_number_of_dices_defense));
			}
			else {
				do {
					cout << "How many dices do you want to attack with? (between 1 and " << max_number_of_dices_attack << ")"
						<< endl;
					cin >> number_of_dices_attack;
				} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
				number_of_dices_defense = max_number_of_dices_defense;
			}

			//Here goes the logic for battle, pairwise comparison of dices


			cout << "** Starting battle **" << endl;

			int number_of_comparisons = min(number_of_dices_attack, number_of_dices_defense);
			vector<int>* attacker_rolls = Dice::sortDsc(number_of_dices_attack);
			vector<int>* defender_rolls = Dice::sortDsc(number_of_dices_defense);

			cout << "Attacker has: " << armies_in_attacking_country << " troups" << endl;
			cout << "Defender has: " << armies_in_defending_country << " troups" << endl;

			cout << "Number of dice comparisons: " << number_of_comparisons << endl;

			for (int i = 0; i < number_of_comparisons; i++) {

				cout << "Roll #" << (i + 1) << ": Attacker rolled a " << attacker_rolls->at(i) << ", defender rolled a "
					<< defender_rolls->at(i) << endl;
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
				player_wishes_to_attack = answer == "y";
			}
		}
		//adjusts number of armies post battle
		attack_source->set_nb_armies(armies_in_attacking_country);
		attack_target->set_nb_armies(armies_in_defending_country);
		if (attacker_won) {
			this->add_country(attack_target);
			defending_player->remove_country(attack_target->get_name());
			cout << player_name << " has conquered " << attack_target->get_name() << endl;
			cout << player_name << " , select the number of troops you want to move to " << attack_target->get_name()
				<< "(between 1 and " << (armies_in_attacking_country - 1) << ")" << endl;
			int armies_moved_to_conquered_country;
			do {
				cin >> armies_moved_to_conquered_country;
			} while (!(armies_moved_to_conquered_country > 0 &&
				armies_moved_to_conquered_country < armies_in_attacking_country));
			attack_source->set_nb_armies((attack_source->get_nb_armies()) - armies_moved_to_conquered_country);
			attack_target->set_nb_armies(armies_moved_to_conquered_country);
		}

		cout << "*** Battle is over ***" << endl;
	}
}

void Player::fortify() {
	int* type = strategy->execute_strategy();
	cout << *type;
	if (*type == 1) {
		string can_player_fortify = this->can_player_fortify(); // Check if player has ability to fortify from the get go
		if (can_player_fortify == "You can fortify!") {
			Country* strongest_target = this->owned_countries->at(0);
			for (int i = 1; i < this->owned_countries->size(); i++) {
				if (strongest_target->get_nb_armies() < this->owned_countries->at(i)->get_nb_armies()) {
					strongest_target = this->owned_countries->at(i);
				}
			}
			Country* target_country = strongest_target;
			for (int i = 0; i < strongest_target->get_adjacent_countries()->size(); i++) {
				Country* source_country = strongest_target->get_adjacent_countries()->at(i);
				string can_player_fortify_source_and_target = this->can_player_fortify(source_country->get_name(),
					target_country->get_name());
				if (can_player_fortify_source_and_target == "Can fortify") {
					int nbr_of_armies_moved = source_country->get_nb_armies();
					if (nbr_of_armies_moved > 0 && nbr_of_armies_moved < source_country->get_nb_armies()) {
						source_country->set_nb_armies(
							source_country->get_nb_armies() -
							nbr_of_armies_moved); // set nbr of armies for source to be source - armies moved
						target_country->set_nb_armies(
							target_country->get_nb_armies() +
							nbr_of_armies_moved); // set nbr of armies for target to be source + armies moved

						cout << "There is now " << source_country->get_nb_armies() << " armies in "
							<< source_country->get_name() << endl;

						cout << "There is now " << target_country->get_nb_armies() << " armies in "
							<< target_country->get_name() << endl;
					}
				} else {
				    cout << this->player_name << " cannot fortify" << endl;
				}
			}
		}
	}
	else if (*type == 2) {
		string can_player_fortify = this->can_player_fortify(); // Check if player has ability to fortify from the get go
		if (can_player_fortify == "You can fortify!") {
			Country* weakest_target = this->owned_countries->at(0);
			for (int i = 1; i < this->owned_countries->size(); i++) {
				if (weakest_target->get_nb_armies() > this->owned_countries->at(i)->get_nb_armies()) {
					weakest_target = this->owned_countries->at(i);
				}
			}
			Country* target_country = weakest_target;
			for (int i = 0; i < weakest_target->get_adjacent_countries()->size(); i++) {
				Country* source_country = weakest_target->get_adjacent_countries()->at(i);
				string can_player_fortify_source_and_target = this->can_player_fortify(source_country->get_name(),
					target_country->get_name());
				if (can_player_fortify_source_and_target == "Can fortify") {
					int nbr_of_armies_moved = source_country->get_nb_armies();
					if (nbr_of_armies_moved > 0 && nbr_of_armies_moved < source_country->get_nb_armies()) {
						source_country->set_nb_armies(
							source_country->get_nb_armies() -
							nbr_of_armies_moved); // set nbr of armies for source to be source - armies moved
						target_country->set_nb_armies(
							target_country->get_nb_armies() +
							nbr_of_armies_moved); // set nbr of armies for target to be source + armies moved

						cout << "There is now " << source_country->get_nb_armies() << " armies in "
							<< source_country->get_name() << endl;

						cout << "There is now " << target_country->get_nb_armies() << " armies in "
							<< target_country->get_name() << endl;
					}
				}
			}
		}
	}
	else if (*type == 3) {
		string answer;
		while (true) { // loop until exit
			cout << "What do you wish to do (fortify, exit) ?" << endl;
			cin >> answer;

			if (answer == "fortify") {
				string can_player_fortify = this->can_player_fortify(); // Check if player has ability to fortify from the get go
				if (can_player_fortify == "You can fortify!") {
					cout << "Here is a list of your owned countries" << endl;

					while (true) {
						cout << "Select the source country" << endl;
						int* source_country_index_ptr = this->print_and_return_index_country_selected();
						cout << "Select the target country" << endl;
						int* target_country_index_ptr = this->print_and_return_index_country_selected();
						string source_country = this->get_player_owned_countries()->at(
							*source_country_index_ptr - 1)->get_name();
						string target_country = this->get_player_owned_countries()->at(
							*target_country_index_ptr - 1)->get_name();
						bool is_source_country_valid = false;
						bool is_target_country_valid = false;

						for (Country* owned_country : *this->owned_countries) {
							if (source_country == owned_country->get_name()) {
								is_source_country_valid = true; // If player owns source country
							}

							if (target_country == owned_country->get_name()) {
								is_target_country_valid = true; // If player owns target country
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
							target_country); // check if user can fortify with source country and target country

						if (can_player_fortify_source_and_target == "Can fortify") {

							while (true) {

								auto* source_country_ptr = new Country();
								auto* target_country_ptr = new Country();
								for (auto& owned_country : *owned_countries) {
									if (owned_country->get_name() == source_country) {
										source_country_ptr = owned_country; // select source country
									}
									if (owned_country->get_name() == target_country) {
										target_country_ptr = owned_country; // select target country
									}
								}

								int nbr_of_armies_moved;
								cout << "Enter the number of armies you wish to move" << endl;
								cout << "Armies in source country: " << source_country_ptr->get_nb_armies() << endl;
								cout << "Armies in target country: " << target_country_ptr->get_nb_armies() << endl;
								cin >> nbr_of_armies_moved;

								if (nbr_of_armies_moved > 0 && nbr_of_armies_moved < source_country_ptr->get_nb_armies()) {
									source_country_ptr->set_nb_armies(
										source_country_ptr->get_nb_armies() -
										nbr_of_armies_moved); // set nbr of armies for source to be source - armies moved
									target_country_ptr->set_nb_armies(
										target_country_ptr->get_nb_armies() +
										nbr_of_armies_moved); // set nbr of armies for target to be source + armies moved

									cout << "There is now " << source_country_ptr->get_nb_armies() << " armies in "
										<< source_country_ptr->get_name() << endl;

									cout << "There is now " << target_country_ptr->get_nb_armies() << " armies in "
										<< target_country_ptr->get_name() << endl;
									break;
								}
								else {
									cout << "Invalid number of armies ! Retry." << endl;
									cout << "Do you wish to exit ? (yes/no)" << endl;
									string wish_to_exit;
									cin >> wish_to_exit;
									if (wish_to_exit == "yes") {
										break;
									}
									else {
										continue;
									}
								}
							}

						}
						else {
							cout << can_player_fortify_source_and_target
								<< endl;
							continue;
						}

						break;
					}
				}
				else {
					cout << can_player_fortify << endl;
					break;
				}
				break;
			}
			else if (answer == "exit")
				break;
			else
				cout << "Enter a valid answer" << endl;
		}
	}
}
void Player::update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus) {
    //TODO
}

void Player::setStrategy(Strategy* newStrategy)
{
	this->strategy = newStrategy;
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
            if (owned_country->get_nb_armies() == 1) { // only 1 country from source, can't fortify
                return "Cannot fortify! Source country only has 1 army";
            }
            for (auto& adjacent_country : *owned_country->get_adjacent_countries()) { // check adjacent country
                if (adjacent_country->get_name() == target_country) {
                    return "Can fortify";
                }
            }
        }
    }
    return "Cannot fortify! Countries aren't adjacent";
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