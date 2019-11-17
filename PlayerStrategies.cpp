#include "PlayerStrategies.h"

void Strategy::fortify()
{
}

void Strategy::attack()
{
}

void Strategy::reinforce()
{
}


void AggressiveAI::fortify() {
	cout << "Aggressive fortify" << endl;
}

void AggressiveAI::reinforce() {
	cout << "Aggressive reinforce" << endl;
	/*
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
	strongestCountry = this->get_player_owned_countries()->at(0);
	for (int i =1; i < this->get_player_owned_countries()->size(); i++){
		if (strongestCountry->get_nb_armies() < player->get_player_owned_countries().at(i)->get_nb_armies())
		{
			strongestCountry = this->get_player_owned_countries()->at(i);
		}
	}

	//place armies at strongest country
	for (int i = 0; i < *num_armies; i++) {
			strongestCountry->add_army();
		}
	}

	cout << this->get_player_name() << " has placed " << *num_armies << " armies at " << strongestCountry->get_name() << endl;
	for (auto& owned_country : *owned_countries) {
		cout << owned_country->get_name() << " has " << owned_country->get_nb_armies() << " armies" << endl;
	}*/
}

void AggressiveAI::attack() {
	cout << "Aggressive attack" << endl;
}

void PassiveAI::fortify() {
	cout << "Passive fortify" << endl;
}

void PassiveAI::reinforce() {
	cout << "Passive reinforce" << endl;

	/*
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
	weakestCountry = this->get_player_owned_countries()->at(0);
	for (int i =1; i < this->get_player_owned_countries()->size(); i++){
		if (weakestCountry->get_nb_armies() > player->get_player_owned_countries().at(i)->get_nb_armies())
		{
			weakestCountry = this->get_player_owned_countries()->at(i);
		}
	}

	//place armies at strongest country
	for (int i = 0; i < *num_armies; i++) {
			weakestCountry->add_army();
		}
	}

	cout << this->get_player_name() << " has placed " << *num_armies << " armies at " << weakestCountry->get_name() << endl;
	for (auto& owned_country : *owned_countries) {
		cout << owned_country->get_name() << " has " << owned_country->get_nb_armies() << " armies" << endl;
	}*/
}

void PassiveAI::attack() {
	cout << "Passive attack" << endl;
}

void Human::fortify() {
	cout << "Human fortify" << endl;
	/*string answer;
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
	}*/
}

void Human::reinforce() {
	cout << "Human reinforce" << endl;

	/*
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
	}*/
}

void Human::attack() {
	cout << "Human attack" << endl;
}