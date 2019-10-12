#include<iostream>
#include <vector>
#include "Map.h"
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
	std::cout << "A default player object was created" << std::endl;
}
Player::Player(std::string playername) {
	this->player_name = playername;
    this->hand = new Hand();
	this->owned_countries = new vector<Country*>;
	this->owned_continents = new vector<Continent*>;
	this->dice_container = new vector<Dice*>;
	countries_owned_bonus = 3;
	continents_owned_bonus = 0;
	std::cout << "A default player object was created" << std::endl;
}
Player::~Player() {
	std::cout << "player " << get_player_name() << " was removed from the game" << std::endl;
};

void Player::add_country(Country* new_country) {
	this->owned_countries->push_back(new_country);
}
void Player::remove_country(std::string removed_country) {
	for (int i = 0; i < owned_countries->size(); i++) {
		if (removed_country.compare((*owned_countries)[i]->get_name())==0) {
			owned_countries->erase(owned_countries->begin() + i);
		}
	}
}
void Player::add_continent(Continent* new_continent) {
	this->owned_continents->push_back(new_continent);
}
void Player::remove_continent(std::string removed_continent) {
	for (int i = 0; i < owned_continents->size(); i++) {
		if (removed_continent.compare((*owned_continents)[i]->get_name())==0) {
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
	cout << "the method fortify has been called by"  << this->get_player_name() << std::endl;
}
void Player::update_bonus(int new_cards_bonus, int new_countries_bonus, int new_continents_bonus) {
	//TODO
}