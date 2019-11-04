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
    //TO do: validation loops should probably be in separate methods
	Country* attack_source = nullptr;
	Country* attack_target = nullptr;
	vector<Country*>* playersCountries = this->get_player_owned_countries();
	vector<Country*> valid_source;

	cout <<this->get_player_name()<<", please select a country to attack from among the following countries:"<<endl;
	for (Country* country_pointer: *playersCountries) {
		if (country_pointer->get_nb_armies > 0) {
			valid_source.push_back(country_pointer);
			cout << country_pointer->get_name() << endl;
		}
	}
	//get the source of the attack
	string players_choice_source = "";
	bool attack_source_valid = false;
	do{
		cin >> players_choice_source;
		for (Country* country_pointer : valid_source) {
			if ((country_pointer->get_name()).compare(players_choice_source) == 0) {
				attack_source_valid = true;
				attack_source = country_pointer;
			}
		}
	} while (!attack_source_valid);
	//get the target of the attack
	cout << this->get_player_name() << ", please select a country to attack from " << players_choice_source<<" among the following countries:" <<endl;
	vector<Country*> valid_target;
	for (Country* country_pointer :*(attack_source->get_adjacent_countries())) {
		if (country_pointer->get_player() == this) {
			valid_target.push_back(country_pointer);
			cout << country_pointer->get_name() << endl;
		}
	}
	string players_choice_target = "";
	bool attack_target_valid = false;
	do {
		cin >> players_choice_target;
		for (Country* country_pointer : *playersCountries) {
			if ((country_pointer->get_name()).compare(players_choice_target) == 0) {
				attack_target_valid = true;
				attack_target = country_pointer;
			}
		}
	} while (!attack_target_valid);
	
	//Now the battle loop
	bool player_wishes_to_attack = true;
	bool battle_is_over = false;
	bool attacker_won = false;
	string answer;

	int armies_in_attacking_country;
	int max_number_of_dices_attack;
	int number_of_dices_attack;
	int armies_in_defending_country;
	int max_number_of_dices_defense;
	int number_of_dices_defense;

	while (player_wishes_to_attack && (!battle_is_over)) {

		armies_in_attacking_country = attack_source->get_nb_armies();
		max_number_of_dices_attack = (armies_in_attacking_country<=4)? armies_in_attacking_country -1:3;
		armies_in_defending_country = attack_target->get_nb_armies();
		max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;

		do {
			cout << "How many dices do you want to attack with? (between 1 and " << max_number_of_dices_attack << ")" << endl;
			cin >> number_of_dices_attack;
		} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
		do {
			cout << "How many dices does " << attack_target ->get_player()->get_player_name()<<" want to to defend with? (between 1 and " << max_number_of_dices_defense << ")" << endl;
			cin >> number_of_dices_defense;
		} while (!(number_of_dices_defense > 0 && number_of_dices_attack <= max_number_of_dices_defense));

		/*
			Here goes the logic for battle, pairwise comparison of dices and updating of troops and potential transfer of ownership of embattled country
		
		*/
		if (!battle_is_over) {
			do {
				cout << this->get_player_name() << " do you wish to continue the attack? (y/n)" << endl;
				cin >> answer;
			} while (!(answer.compare("y") == 0 || answer.compare("n") == 0));
			if (answer.compare("y") == 0)
				player_wishes_to_attack = true;
			else
				player_wishes_to_attack = false;
		}
	}
}

void Player::fortify() {
    cout << "the method fortify has been called by" << this->get_player_name() << std::endl;
}

void Player::update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus) {
    //TODO
}