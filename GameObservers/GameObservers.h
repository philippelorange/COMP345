#pragma once

#include <list>

class Player;

using namespace std;

enum GamePhase {
    reinforcement, attack, fortify, not_is_turn
};

// Observers

class GameObservers {
public:
    ~GameObservers();

    virtual void update(GamePhase game_phase) = 0;

protected:
    GameObservers();
};

// Concrete observers

class PhaseObserver : public GameObservers {
public:

    explicit PhaseObserver(Player* player);

    void update(GamePhase game_phase) override;

private:

    Player* player;

    void display_game_phase(GamePhase game_phase);

    void display_relevant_info(GamePhase game_phase);

    string get_phase_string(GamePhase game_phase);
};

// Subjects

class Observable {
public:
    Observable();

    ~Observable();

    virtual void detach(GameObservers* observer);

    virtual void notify(GamePhase game_phase);

    virtual void attach(GameObservers* observer);

private:
    list<GameObservers*>* game_observers;

};

