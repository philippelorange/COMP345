#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Player/Player.h"

class Game {
public:
    void start();
    Map* get_map();
    vector<Player*>* get_players();
    Deck* get_deck();
private:
    static void print_intro();
    void select_map();
    void create_players();
    void create_deck();
    Deck* _deck;
    Map* _selected_map;
    vector<Player*>* _players;
};