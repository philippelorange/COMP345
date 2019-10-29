#include "GameEngine.h"
#include <iostream>
using namespace std;

int main() {
    Game* game = new Game();
    game->start();

    cout << "Game has been set up. Let's check the map, players, and deck:" << endl;

    cout << "Map: " << game->get_map()->get_name() << endl;
    cout << "\t -Number of countries: " << game->get_map()->get_countries()->size() << endl;
    cout << "Number of players: " << game->get_players()->size() << endl;
    for(auto& p : *game->get_players()) {
        cout << "\t -" << p->get_player_name() << endl;
    }
    cout << "Number of cards in the deck: " << game->get_deck()->get_deck_cards()->size() << endl;
}