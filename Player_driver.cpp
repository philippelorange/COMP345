#include <iostream>
#include "Player.h"
#include "Cards/Cards.h"
#include "Map.h"
#include "Dice/Dice.h"
#include <time.h>

using namespace std;

int main() {
	cout << "Creating a default player..." << endl;
	Player* player_1 = new Player;
	cout << "The player " << player_1->get_player_name() << " has been added to the game" << endl;
	cout << "Prompt for player creation will soon appear..." << endl;
	cout << "Please provide a name for the player you wish to add to the game... " << endl;
	string player_2_name;
	cin >> player_2_name;
	Player* player_2 = new Player(player_2_name);
	player_2->fortify();
	player_2->attack();
	player_2->reinforce();


	cout << "Creating a Deck..." << endl;
	auto* cards = new vector<Card*>;
	srand((unsigned)time(NULL)); // random seed to use unpredictable numbers

	cout << "Creating cards.." << endl;

	int nbr_cards = 42; // TODO: get number of cards from map instead of 42 (hardcoded)

	while (nbr_cards % 3 != 0) { // get number divisible by 3 for purpose of this assignment
		nbr_cards--;
	}

	for (int i = 0, type_number = 0; i < nbr_cards; i++, type_number++) { // create cards from map
		type_number == 3 ? type_number = 0 : type_number;
		Type* type_ptr = new Type;
		cards->push_back(Card::factory_card(type_ptr, type_number)); // add reference of card to vector
	}

	int* infantry_counter = new int(0);
	int* cavalry_counter = new int(0);
	int* artillery_counter = new int(0);

	for (Card* card : *cards) {
		cout << "Card " << card->get_type_name() << " created" << endl;
		if (card->get_type_name() == "infantry") // count how many cards in deck
			(*infantry_counter)++;
		else if (card->get_type_name() == "cavalry")
			(*cavalry_counter)++;
		else
			(*artillery_counter)++;
	}

	cout << "Successfully created " << *infantry_counter << " infantry cards" << endl;
	cout << "Successfully created " << *cavalry_counter << " cavalry cards" << endl;
	cout << "Successfully created " << *artillery_counter << " artillery cards" << endl;

	delete (infantry_counter); // delete unnecessary counter at this point
	delete (cavalry_counter);
	delete (artillery_counter);

	Deck* deck = new Deck(cards);
	deck->draw(player_2->get_hand());

	cout << player_2->get_player_name() << " has drawn" << endl;
    player_2->get_hand()->print_card_in_hand();

	Country* Canada = new Country("Canada",nullptr);
	player_2->add_country(Canada);
	cout << player_2->get_player_name()<<" has the following country: "<< player_2->get_player_owned_countries()->front()->get_name()<<endl;
	player_2->remove_country("Canada");
	if(player_1->get_player_owned_countries()->size()==0)
	cout << player_2->get_player_name() <<" No longer has any country" << endl;
}