#include <iostream>
#include <filesystem>

#include "GameEngine.h"
#include "Map/Map.h"
#include "Map/MapLoader/MapLoader.h"
#include "Player/Player.h"

using namespace std;

namespace fs = std::filesystem;

void Game::start() {
    print_intro();
    select_map();
    create_players();
    create_deck();
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

    auto* mapLoader = new MapLoader();
    for(auto& p: std::filesystem::directory_iterator("../Map/Maps")) {
        if (p.path().string().substr(p.path().string().find_last_of('.') + 1) == "map") {
            Map *map = mapLoader->read_map(p.path());
            if (map != nullptr && map->validate_continent_singularity() && map->validate_connected_graph()) {
                map->set_name(p.path().filename());
                valid_files.push_back(map);
            } else {
                invalid_files.push_back(p.path().filename());
            }
        }
    }

    int user_choice = -1;
    while(user_choice < 1 || user_choice > valid_files.size()) {
        cout << "Please select a map from the following list: " << endl;
        for(int i=0; i< valid_files.size(); i++) {
            cout << "(" << i+1 << ") " << valid_files.at(i)->get_name() << endl;
        }

        if(!invalid_files.empty()) {
            cout << endl << "Warning: The following maps were also found, but are invalid:" << endl;
            for(auto& s : invalid_files) {
                cout << " - " << s << endl;
            }
        }

        cin >> user_choice;
        if(user_choice < 1 || user_choice > valid_files.size()) {
            cout << "Invalid input." << endl;
        }
    }
    _selected_map = valid_files.at(user_choice-1);
    cout << "You have selected: " << _selected_map->get_name() << endl;
}

void Game::create_players() {
    int nb_players = -1;
    while(nb_players < 2 || nb_players > 6) {
        cout << "How many players want to play?" << endl;
        cin >> nb_players;
        if(nb_players > 6 || nb_players < 2) {
            cout << "Invalid number of players." << endl;
        }
    }

    _players = new vector<Player*>();

    for(int i=0; i<nb_players; i++) {
        cout << "Enter name for player #" << i+1 << ":" << endl;
        string name;
        cin >> name;
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

Map* Game::get_map() {
    return _selected_map;
}

vector<Player*>* Game::get_players() {
    return _players;
}

Deck* Game::get_deck() {
    return _deck;
}