#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Player/Player.h"

class Game {
public:
    void start();
    void game_setup();
    void startup_phase();
    Map* get_map();
    vector<Player*>* get_players();
    Deck* get_deck();
private:
    static void print_intro();
    void select_map();
    void create_players();
    void create_deck();
    void determine_order();
    void assign_countries();
    void place_armies();
    Deck* _deck;
    Map* _selected_map;
    vector<Player*>* _players;
};