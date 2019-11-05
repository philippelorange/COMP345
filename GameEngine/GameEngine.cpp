#include <iostream>
#include <filesystem>
#include <algorithm>
#include <random>

#include "GameEngine.h"
#include "Map/Map.h"
#include "Map/MapLoader/MapLoader.h"
#include "Player/Player.h"

using namespace std;

namespace fs = std::filesystem;

void Game::start() {
    game_setup();
    startup_phase();
    game_loop();
}

void Game::game_setup() {
    print_intro();
    select_map();
    create_players();
    create_deck();
}

void Game::startup_phase() {
    determine_order();
    assign_countries();
    place_armies();
}

void Game::game_loop() {
    cout << "*** Time to play Risk! ***" << endl << endl;

    bool game_over = false;
    while (!game_over) {
        for (auto& _player : *_players) {
            cout << _player->get_player_name() << ", it's your turn to play." << endl;
            reinforcements_phase(_player);
            attack_phase(_player);
            fortification_phase(_player);
            if (has_victory()) {
                game_over = true;
                break;
            }
        }
    }

    cout << "*** The game is over! ***" << endl;
    cout << "The champion is: " << _selected_map->get_countries()->at(0)->get_player()->get_player_name() << "!!!" << endl;
}

void Game::setup_winning_game() {
    game_setup();
    startup_phase();
    for (auto& c : *_selected_map->get_countries()) {
        c->set_player(_players->at(0));
    }
    game_loop();
}

void Game::print_intro() {
    cout << ".------..------..------..------." << endl;
    cout << "|R.--. ||I.--. ||S.--. ||K.--. |" << endl;
    cout << R"(| :(): || (\/) || :/\: || :/\: |)" << endl;
    cout << R"(| ()() || :\/: || :\/: || :\/: |)" << endl;
    cout << "| '--'R|| '--'I|| '--'S|| '--'K|" << endl;
    cout << "`------'`------'`------'`------'" << endl << endl;

    cout << "Welcome to Risk!" << endl << endl;
}

void Game::select_map() {
    vector<Map*> valid_files;
    vector<string> invalid_files;

    //The following block will find all files ending with .map, and split up the valid from the invalid ones,
    //for the user only to be able to select a valid file.
    auto* mapLoader = new MapLoader();
    for (auto& p: std::filesystem::directory_iterator("../Map/Maps")) {
        if (p.path().string().substr(p.path().string().find_last_of('.') + 1) == "map") {
            Map* map = mapLoader->read_map(p.path());
            if (map != nullptr && map->validate_continent_singularity() && map->validate_connected_graph()) {
                map->set_name(p.path().filename());
                valid_files.push_back(map);
            } else {
                invalid_files.push_back(p.path().filename());
            }
        }
    }

    int user_choice = -1;
    while (user_choice < 1 || user_choice > valid_files.size()) {
        cout << "Please select a map from the following list: " << endl;
        for (int i = 0; i < valid_files.size(); i++) {
            cout << "(" << i + 1 << ") " << valid_files.at(i)->get_name() << endl;
        }

        if (!invalid_files.empty()) {
            cout << endl << "Warning: The following maps were also found, but are invalid:" << endl;
            for (auto& s : invalid_files) {
                cout << " - " << s << endl;
            }
        }

        cin >> user_choice;
        if (cin.fail() || user_choice < 1 || user_choice > valid_files.size()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }
    _selected_map = valid_files.at(user_choice - 1);
    cout << "You have selected: " << _selected_map->get_name() << endl;
}

void Game::create_players() {
    int nb_players = -1;
    while (nb_players < 2 || nb_players > 6) {
        cout << "How many players want to play?" << endl;
        cin >> nb_players;
        if (cin.fail() || nb_players > 6 || nb_players < 2) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid number of players." << endl;
        }
    }

    _players = new vector<Player*>();

    for (int i = 0; i < nb_players; i++) {
        string name;
        bool valid_name = false;
        while (!valid_name) {
            cout << "Enter name for player #" << i + 1 << ":" << endl;
            cin >> name;
            if (i == 0) {
                valid_name = true; //For the first player, no need to check for other players having the same name
            } else {
                for (auto& p : *_players) {
                    if (p->get_player_name() == name) {
                        cout << "Another player already has this name!" << endl;
                        valid_name = false;
                        break;
                    } else {
                        valid_name = true;
                    }
                }
            }
        }
        _players->push_back(new Player(name));
    }
}

void Game::create_deck() {
    auto* cards = new vector<Card*>;

    int nbr_cards = _selected_map->get_countries()->size();

    for (int i = 0, type_number = 0; i < nbr_cards; i++, type_number++) { // create cards from map
        type_number == 3 ? type_number = 0 : type_number;
        Type* type_ptr = new Type;
        cards->push_back(Card::factory_card(type_ptr, type_number)); // add reference of card to vector
    }

    _deck = new Deck(cards);
}

void Game::determine_order() {
    std::random_device rd;
    auto re = default_random_engine(rd());
    //Use the std library to shuffle the player array
    std::shuffle(std::begin(*_players), end(*_players), re);
    cout << "Order of play has been determined: " << endl;
    for (auto& p : *_players) {
        cout << "\t -" << p->get_player_name() << endl;
    }
}

void Game::assign_countries() {
    cout << "Now assigning random countries in a round-robin fashion:" << endl;

    //Create a vector of integers representing each country's index in the vector. We
    //will shuffle this index vector to randomly assign countries to each player.
    vector<int> country_indexes(_selected_map->get_countries()->size());

    std::iota(std::begin(country_indexes), std::end(country_indexes), 0);

    std::random_device rd;
    auto re = default_random_engine(rd());
    std::shuffle(std::begin(country_indexes), end(country_indexes), re);

    for (int i = 0; i < _selected_map->get_countries()->size(); i++) {
        _players->at(i%_players->size())->add_country(_selected_map->get_countries()->at(country_indexes.at(i)));
        _selected_map->get_countries()->at(country_indexes.at(i))->add_army();
    }

    cout << "Countries have been assigned! Here is the outcome: " << endl;
    for (auto& p : *_players) {
        cout << "\t- " << p->get_player_name() << ": " << endl;
        for (auto& c : *p->get_player_owned_countries()) {
            cout << "\t \t * " << c->get_name() << endl;
        }
    }
}

void Game::place_armies() {
    cout << "Time to place the armies!" << endl;

    int nb_armies = 0;

    switch(_players->size()) {
        case 2 : nb_armies = 40; break;
        case 3 : nb_armies = 35; break;
        case 4 : nb_armies = 30; break;
        case 5 : nb_armies = 25; break;
        case 6 : nb_armies = 20; break;
    }

    //loop until the number of armies left is 0
    for (int i = 0; i < nb_armies; i++) {
        for (auto& p : *_players) {
            int selection = -1;
            while (selection < 1 || selection > p->get_player_owned_countries()->size()) {
                cout << "\t" << p->get_player_name() << ", please place an army. You have " << (nb_armies - i)
                     << " left" << endl;
                for (int k = 0; k < p->get_player_owned_countries()->size(); k++) {
                    cout << "\t \t (" << (k + 1) << ") " << p->get_player_owned_countries()->at(k)->get_name() << endl;
                }

                cin >> selection;
                if (cin.fail() || selection < 1 || selection > p->get_player_owned_countries()->size()) {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cout << "Invalid input." << endl;
                }
            }
            p->get_player_owned_countries()->at(selection - 1)->add_army();
        }
    }
}

void Game::reinforcements_phase(Player* p) {
    cout << "*** " << p->get_player_name() << "'s Reinforcements phase ***" << endl;
    string t;
    cin >> t;
}

void Game::attack_phase(Player* p){
	string player_name = p->get_player_name();
    cout << "*** " << player_name << "'s Attack phase ***" << endl;
	string answer;

	//Check if player wants to initiate an attack
	bool player_wants_to_attack;
    do {
        cout << player_name << ", do you want to attack? (y/n)" << endl;
        cin >> answer;
    } while (!(answer == "y" || answer == "n"));
    if(answer == "y")
        player_wants_to_attack = true;
    else
        player_wants_to_attack = false;

	//The attack phase is a loop that prompts the user for intent to attack and calls the attack method, as long as he wishes to do so
	while (player_wants_to_attack) {
		p->attack();
		do {
			cout << player_name << ", would you like to initiate another attack? (y/n)" << endl;
			cin >> answer;
		} while (!(answer == "y" || answer == "n"));
		if(answer == "y")
			player_wants_to_attack = true;
		else
			player_wants_to_attack = false;
	}
}

void Game::fortification_phase(Player* p) {
    cout << "*** " << p->get_player_name() << "'s Fortification phase ***" << endl;
    p->fortify();
}

Player* Game::has_victory() {
    Player* p = _selected_map->get_countries()->at(0)->get_player();
    for (auto& c : *_selected_map->get_countries()) {
        if (c->get_player()->get_player_name() != p->get_player_name()) {
            return nullptr;
        }
    }
    return p;
}

Map* Game::get_map() {
    return _selected_map;
}

vector<Player*>* Game::get_players() {
    return _players;
}

Deck* Game::get_deck() {
    return _deck;
}