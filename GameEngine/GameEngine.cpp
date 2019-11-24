#include <iostream>
#include <filesystem>
#include <algorithm>
#include <random>
#include <numeric>

#include "GameEngine.h"
#include "Map/Map.h"
#include "Map/MapLoader/MapLoader.h"
#include "Player/Player.h"
#include "PlayerStrategies/PlayerStrategies.h"

using namespace std;

namespace fs = std::filesystem;

Player* Game::play_tournament_game(vector<Strategy *> *strategies, Map map, int num_turns) {
    _selected_map = &map;
    _num_turns = &num_turns;
    create_deck();
    _players = new vector<Player*>();
    for(auto& s : *strategies) {
        _players->push_back(new Player(s->get_name(), _deck, s));
    }

    startup_phase();
    return game_loop(num_turns);
}

void Game::start() {
    game_setup();
    startup_phase();
    game_loop(-1);
}

void Game::game_setup() {
    print_intro();
    select_map();
    create_deck();
    create_players();

}

void Game::startup_phase() {
    determine_order();
    assign_countries();
    place_armies();
}

Player* Game::game_loop(int num_turns) {
    cout << "*** Time to play Risk! ***" << endl << endl;

    bool game_over = false;
    while (!game_over && num_turns != 0) {
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
        num_turns--;
    }

    cout << "*** The game is over! ***" << endl;

    if(game_over) {
        auto* winner = _selected_map->get_countries()->at(0)->get_player();
        cout << "The champion is: " << winner->get_player_name() << "!!!"
             << endl;
        return winner;
    } else {
        cout << "Turn limit exceeded; game ended in a draw." << endl;
        return nullptr;
    }
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
    for (auto& p: std::filesystem::directory_iterator("../Map/Maps/Domination")) {
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

    mapLoader = new ConquestAdapter();
    for (auto& p: std::filesystem::directory_iterator("../Map/Maps/Conquest")) {
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
    while (nb_players < 2 || nb_players > 5) {
        cout << "How many players want to play (there will one AI by default)?" << endl;
        cin >> nb_players;
        if (cin.fail() || nb_players > 6 || nb_players < 2) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid number of players." << endl;
        }
    }

    _players = new vector<Player*>();

    for (int i = 0; i < nb_players - 1; i++) {
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

        auto* player = new Player(name, this->_deck);
        auto* phase_observer = new ConcretePhaseObserver(player);
        player->attach(phase_observer);
        _players->push_back(player);
    }

    string name_ai = "default_ai";
    auto* player_ai = new Player(name_ai, this->_deck, new AggressiveStrategy());
    auto* phase_observer_ai = new ConcretePhaseObserver(player_ai);
    player_ai->attach(phase_observer_ai);
    _players->push_back(player_ai);
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
        _players->at(i % _players->size())->add_country(_selected_map->get_countries()->at(country_indexes.at(i)));
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

    switch (_players->size()) {
        case 2 :
            nb_armies = 5;
            break;
        case 3 :
            nb_armies = 5;
            break;
        case 4 :
            nb_armies = 5;
            break;
        case 5 :
            nb_armies = 5;
            break;
        case 6 :
            nb_armies = 5;
            break;
    }

    nb_armies = 5;

    //loop until the number of armies left is 0
    for (int i = 0; i < nb_armies; i++) {
        for (auto& p : *_players) {
            cout << p->get_player_name() << "'s turn to place an army. " << (nb_armies -i) << " left to place." << endl;
            p->get_strategy()->place_army(p->get_player_owned_countries());
        }
    }
}

void Game::reinforcements_phase(Player* p) {
    p->reinforce();
}

void Game::attack_phase(Player* p) {
    p->attack();
}

void Game::fortification_phase(Player* p) {
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

Tournament::Tournament() {
    _maps = new vector<Map*>();
    _player_strategies = new vector<Strategy*>();
}

void Tournament::start() {
    cout << "*********************************************" << endl;
    cout << "*                                           *" << endl;
    cout << "*              TOURNAMENT MODE              *" << endl;
    cout << "*                                           *" << endl;
    cout << "*********************************************" << endl << endl;

    cout << "Welcome to Tournament mode!" << endl;

    select_maps();

    select_players();

    select_number_games();

    select_number_turns();

    cout << "The tournament is about to start! Here are your settings:" << endl;
    cout << "\tMaps:" << endl;
    for(auto& m : *_maps) {
        cout << "\t\t" << m->get_name() << endl;
    }
    cout << "\tPlayers:" << endl;
    for(auto& p : *_player_strategies) {
        cout << "\t\t" << p->get_name() << endl;
    }
    cout << "\tNumber of games: " << endl << "\t\t" <<*_nb_games << endl;
    cout << "\tNumber of turns: " << endl << "\t\t" <<*_nb_turns << endl << endl;

    cout << "Press any key when you are ready to start..." << endl;
    int x; cin >> x;

    _nb_maps = new int(_maps->size());
    _winning_players = new string*[*_nb_maps];
    for(int i=0; i<*_nb_maps; i++) {
        _winning_players[i] = new string[*_nb_games];
        for(int j=0; j<*_nb_games; j++){
            Game* game = new Game();
            auto* winner = game->play_tournament_game(_player_strategies, *_maps->at(0), *_nb_turns);
            if(winner == nullptr) {
                _winning_players[i][j] = "Draw";
            } else {
                _winning_players[i][j] = winner->get_strategy()->get_name();
            }

            delete game;
        }
    }

    print_results();
}

void Tournament::select_maps() {
    cout << "Please choose the number of maps (1-5):" << endl;
    int selection = -1;
    while (selection < 1 || selection > 5) {
        cin >> selection;
        if (cin.fail() || (selection < 1 || selection > 2)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    cout << "SELECTED MAPS:" << endl;
    for(auto& m : *_maps) {
        cout << "\t" << m->get_name() << endl;
    }

    vector<Map*> valid_files;
    vector<string> invalid_files;

    //The following block will find all files ending with .map, and split up the valid from the invalid ones,
    //for the user only to be able to select a valid file.
    auto* mapLoader = new MapLoader();
    for (auto& p: std::filesystem::directory_iterator("../Map/Maps/Domination")) {
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

    mapLoader = new ConquestAdapter();
    for (auto& p: std::filesystem::directory_iterator("../Map/Maps/Conquest")) {
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

    for(int i=0; i<selection; i++) {
        int user_choice = -1;
        while (user_choice < 1 || user_choice > valid_files.size()) {
            cout << "Please select map " << (i+1) << "/" << selection << " from the following list: " << endl;
            for (int j = 0; j < valid_files.size(); j++) {
                cout << "(" << j + 1 << ") " << valid_files.at(j)->get_name() << endl;
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
        cout << "You have selected: " << valid_files.at(user_choice - 1)->get_name() << endl << endl;
        _maps->push_back(valid_files.at(user_choice - 1));
        valid_files.erase(valid_files.begin()+user_choice-1);
    }
}

void Tournament::select_players() {
    cout << "Please choose the number of players (2-4):" << endl;
    int selection = -1;
    while (selection < 2 || selection > 4) {
        cin >> selection;
        if (cin.fail() || (selection < 2 || selection > 4)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    string strats[] = {"Aggressive", "Benevolent", "Random", "Cheater"};
    vector<string> strategies(strats, strats + sizeof(strats) / sizeof(string));

    for(int i=0; i<selection; i++) {
        int user_choice = -1;
        while (user_choice < 1 || user_choice > strategies.size()) {
            cout << "Please select player " << (i+1) << "/" << selection << " from the following list: " << endl;
            for (int j = 0; j < strategies.size(); j++) {
                cout << "(" << j + 1 << ") " << strategies.at(j) << endl;
            }

            cin >> user_choice;
            if (cin.fail() || user_choice < 1 || user_choice > strategies.size()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input." << endl;
            }
        }
        auto choice = strategies.at(user_choice - 1);
        cout << "You have selected: " << choice << endl << endl;
        if(choice == "Aggressive") {
            _player_strategies->push_back(new AggressiveStrategy());
        } else if(choice == "Benevolent") {
            _player_strategies->push_back(new BenevolentStrategy());
        } else if(choice == "Random") {
            //_player_strategies->push_back(new RandomStrategy());
        } else if(choice == "Cheater") {
            //_player_strategies->push_back(new CheaterStrategy());
        }

        strategies.erase(strategies.begin()+user_choice-1);
    }
}

void Tournament::select_number_games() {
    cout << "Please choose how many games are to be played on each map (1-5):" << endl;
    int selection = -1;
    while (selection < 1 || selection > 5) {
        cin >> selection;
        if (cin.fail() || (selection < 1 || selection > 5)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    cout << "You have selected " << selection << " game(s)." << endl << endl;
    _nb_games = new int(selection);
}

void Tournament::select_number_turns() {
    cout << "Please choose how many turns are to be played each game (10-50):" << endl;
    int selection = -1;
    while (selection < 10 || selection > 50) {
        cin >> selection;
        if (cin.fail() || (selection < 10 || selection > 50)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    cout << "You have selected " << selection << " turns." << endl << endl;
    _nb_turns = new int(selection);
}

void Tournament::print_results() {
    cout << "*********************************" << endl;
    cout << "             RESULTS             " << endl;
    cout << "*********************************" << endl;

    cout << "M: ";
    for(auto& m : *_maps) {
        cout << m->get_name() << "; ";
    }
    cout << endl << "P: ";
    for(auto& p : *_player_strategies) {
        cout << p->get_name() << "; ";
    }
    cout << endl << "G: " << *_nb_games << endl;
    cout << "D: " << *_nb_turns << endl;

    cout << "|----------";
    for(int i=0; i < *_nb_games; i++) {
        cout << "-------------";
    }
    cout << "|" << endl;

    cout << "|          ";
    for(int i=0; i < *_nb_games; i++) {
        cout << "| Game " << i+1 << "     ";
    }
    cout << "|" << endl;

    cout << "|----------";
    for(int i=0; i < *_nb_games; i++) {
        cout << "-------------";
    }
    cout << "|" << endl;

    for(int i=0; i<*_nb_maps; i++) {
        cout << "| Map " << i + 1 << "    ";
        for (int j = 0; j < *_nb_games; j++) {
            string next_line = "| " + _winning_players[i][j];
            cout << left << setw(13) << next_line;
        }
        cout << "|" << endl << "|----------";
        for (int i = 0; i < *_nb_games; i++) {
            cout << "-------------";
        }
        cout << "|" << endl;
    }
}
void GameEngine::start() {
    cout << "Welcome to Risk!" << endl << endl;

    int selection = -1;
    while (selection != 1 && selection != 2) {
        cout << "Would you like to play a single game, or start a tournament?" << endl;
        cout << "\t \t (1) Single Game" << endl;
        cout << "\t \t (2) Tournament" << endl;

        cin >> selection;
        if (cin.fail() || (selection != 1 && selection != 2)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
        }
    }

    if(selection == 1) {
        Game* game = new Game();
        game->start();
    } else {
        auto* tournament = new Tournament();
        tournament->start();
    }
}