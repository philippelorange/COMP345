#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Player/Player.h"

class Game {
public:
    void start();
    void game_setup();
    void startup_phase();
    void game_loop();

    //Methods only used for runner
    void setup_winning_game();
    Map* get_map();
    vector<Player*>* get_players();
    Deck* get_deck();
private:
    static void print_intro();

    //Game start
    void select_map();
    void create_players();
    void create_deck();

    //Startup Phase
    void determine_order();
    void assign_countries();
    void place_armies();

    //Main Game loop
    void reinforcements_phase(Player* p);
    void attack_phase(Player* p);
    void fortification_phase(Player* p);
    Player* has_victory();

    Deck* _deck;
    Map* _selected_map;
    vector<Player*>* _players;
};