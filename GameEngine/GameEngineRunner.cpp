#include "GameEngine.h"
#include <iostream>
using namespace std;

int main() {
    Game* game = new Game();
    game->start();

    cout << "*********************************************************************" << endl << endl;
    cout << "Game has been set up. Let's check the map, players, and deck:" << endl;

    cout << "Map: " << game->get_map()->get_name() << endl;
    cout << "Number of countries: " << game->get_map()->get_countries()->size() << endl;
    for(auto& c: *game->get_map()->get_countries()) {
        cout << "\t -" << c->get_name() << ", player: " << c->get_player()->get_player_name() << ", number of armies: " << c->get_nb_armies() << endl;
    }
    cout << "Number of players: " << game->get_players()->size() << endl;
    for(auto& p : *game->get_players()) {
        cout << "\t -" << p->get_player_name() << endl;
        for(auto& c : *p->get_player_owned_countries())
        cout << "\t \t * " << c->get_name() << ", number of armies: " << c->get_nb_armies() << endl;
    }
    cout << "Number of cards in the deck: " << game->get_deck()->get_deck_cards()->size() << endl;
}