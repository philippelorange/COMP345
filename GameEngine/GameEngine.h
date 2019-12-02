#include "Cards/Cards.h"
#include "Map/Map.h"
#include "Player/Player.h"

class Game {
public:

    Player* play_tournament_game(vector<Strategy*>* strategies, Map map, int num_turns);

    void start();

    void game_setup();

    void startup_phase();

    Player* game_loop(int num_turns);

    //Startup Phase
    void determine_order();

    void assign_countries();

    void place_armies();

    //Main Game loop
    void reinforcements_phase(Player* p);

    void attack_phase(Player* p);

    void fortification_phase(Player* p);

    Player* has_victory();

    Map* get_map();

    vector<Player*>* get_players();

    Deck* get_deck();

    void remove_player(Player* removed_player);

private:
    static void print_intro();

    //Game start
    void select_map();

    void create_players();

    void create_deck();


    Deck* _deck;
    Map* _selected_map;
    vector<Player*>* _players;
    int* _num_turns;
};

class Tournament {
public:
    Tournament();

    void start();

private:
    vector<Map*>* _maps;
    vector<Strategy*>* _player_strategies;
    int* _nb_games;
    int* _nb_turns;
    int* _nb_maps;
    string** _winning_players;

    void select_maps();

    void select_players();

    void select_number_games();

    void select_number_turns();

    void print_results();

};

class GameEngine {
public:
    void start();
};