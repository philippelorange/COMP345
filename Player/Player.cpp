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
	auto* valid_sources = new vector<Country*>();
	auto* valid_target = new vector<Country*>();
	Player* defending_player;

    for (auto& c: *owned_countries) {
        if (c->get_nb_armies() > 1) {
            valid_sources->push_back(c);
        }
    }

    if(valid_sources->empty()) {
        cout << "You do not have any countries capable of performing an attack at this time." << endl;
        return;
    }

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
	
	//Now the battle loop
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

	while (player_wishes_to_attack && (!battle_is_over)) {

	    max_number_of_dices_attack = (armies_in_attacking_country<=4)? armies_in_attacking_country -1:3;
		max_number_of_dices_defense = (armies_in_defending_country <= 2) ? armies_in_defending_country : 2;

		do {
			cout << "How many dices do you want to attack with? (between 1 and " << max_number_of_dices_attack << ")" << endl;
			cin >> number_of_dices_attack;
		} while (!(number_of_dices_attack > 0 && number_of_dices_attack <= max_number_of_dices_attack));
		do {
			cout << "How many dices does " << defending_player->get_player_name()<<" want to to defend with? (between 1 and " << max_number_of_dices_defense << ")" << endl;
			cin >> number_of_dices_defense;
		} while (!(number_of_dices_defense > 0 && number_of_dices_defense <= max_number_of_dices_defense));

		/*
			Here goes the logic for battle, pairwise comparison of dices and updating of troops and potential transfer of ownership of embattled country
		
		*/
		cout << "** Starting battle **" << endl;

		int number_of_comparisons = min(number_of_dices_attack, number_of_dices_defense);
		vector<int>* attacker_rolls = Dice::sortDsc(number_of_dices_attack);
        vector<int>* defender_rolls = Dice::sortDsc(number_of_dices_defense);

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

		if (armies_in_attacking_country == 1)
			battle_is_over = true;
		
		if (armies_in_defending_country == 0) {
			battle_is_over = true;
			attacker_won = true;
		}
		
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

		cout << "*** Battle is over ***" << endl;
	}
}

void Player::fortify() {
    cout << "the method fortify has been called by" << this->get_player_name() << std::endl;
}

void Player::update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus) {
    //TODO
}