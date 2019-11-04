#include "GameEngine.h"
#include <iostream>

using namespace std;

int main() {

//    Game* game = new Game();
//    game->game_setup();
//
//    cout << "*********************************************************************" << endl << endl;
//    cout << "Game has been set up. Let's check the map, players, and deck:" << endl;
//
//    cout << "Map: " << game->get_map()->get_name() << endl;
//    cout << "Number of countries: " << game->get_map()->get_countries()->size() << endl;
//
//    cout << "Number of players: " << game->get_players()->size() << endl;
//    for(auto& p : *game->get_players()) {
//        cout << "\t -" << p->get_player_name() << endl;
//    }
//    cout << "Number of cards in the deck: " << game->get_deck()->get_deck_cards()->size() << endl;


    //<----------------------------------------------------------------------------------------------------------------->
    //<----------------------------------------------------------------------------------------------------------------->


//    Game* game = new Game();
//    game->game_setup();
//    game->startup_phase();
//
//    cout << "*********************************************************************" << endl << endl;
//    cout << "Game has been set up. Let's check the map, players, and deck:" << endl;
//
//    cout << endl << "Map: " << game->get_map()->get_name() << endl;
//    cout << endl << "Number of countries: " << game->get_map()->get_countries()->size() << endl;
//    for(auto& c: *game->get_map()->get_countries()) {
//        cout << "\t -" << c->get_name() << ", player: " << c->get_player()->get_player_name() << ", number of armies: " << c->get_nb_armies() << endl;
//    }
//
//    cout << endl << "Number of players: " << game->get_players()->size() << endl;
//    for(auto& p : *game->get_players()) {
//        cout << "\t -" << p->get_player_name() << endl;
//        for(auto& c : *p->get_player_owned_countries())
//            cout << "\t \t * " << c->get_name() << ", number of armies: " << c->get_nb_armies() << endl;
//    }
//    cout << endl << "Number of cards in the deck: " << game->get_deck()->get_deck_cards()->size() << endl;


    //<----------------------------------------------------------------------------------------------------------------->
    //<----------------------------------------------------------------------------------------------------------------->

    /*
    Game* game = new Game();
    game->setup_winning_game();
    */

    /*
    game = new Game();
    game->game_setup();
    game->startup_phase();
    game->game_loop();
    */


//  Part 5
    Game* game = new Game();
    game->game_setup();
    game->determine_order();
    game->assign_countries();

    cout << "Time to place the armies!" << endl;

    int nb_armies = 5;

    //loop until the number of armies left is 0
    for (int i = 0; i < nb_armies; i++) {
        for (auto& p : *game->get_players()) {
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

    game->game_loop();
}