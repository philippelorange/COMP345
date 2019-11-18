#include <iostream>
#include "GameObservers.h"
#include "Player/Player.h"

// GameObservers
GameObservers::~GameObservers() = default;

GameObservers::GameObservers() = default;

//Concrete observers

PhaseObserver::PhaseObserver(Player* player) {
    this->player = player;
}

void PhaseObserver::display_game_phase(GamePhase game_phase) {
    string current_phase = get_phase_string(game_phase);
    cout << this->player->get_player_name() << " phase: " << current_phase << endl;
}

void PhaseObserver::display_relevant_info(GamePhase game_phase) {
    string current_phase = get_phase_string(game_phase);
    if (current_phase == "not is turn") {
        cout << "***** End of " << this->player->get_player_name() << " turn !! *****" << endl;
    } else {
        cout << current_phase << " relevant info:" << endl;
    }
}

void PhaseObserver::update(GamePhase game_phase) {
    this->display_game_phase(game_phase);
    this->display_relevant_info(game_phase);
}

string PhaseObserver::get_phase_string(GamePhase game_phase) {
    string type_name[] = {"reinforcement", "attack", "fortify", "not is turn"};
    return type_name[game_phase];
}

// Subject

Observable::Observable() {
    this->game_observers = new list<GameObservers*>;
};

Observable::~Observable() {
    delete game_observers;
};

void Observable::attach(GameObservers* observer) {
    this->game_observers->push_back(observer);
}

void Observable::detach(GameObservers* observer) {
    this->game_observers->remove(observer);
}

void Observable::notify(GamePhase game_phase) {
    auto iterator = this->game_observers->begin();
    for (; iterator != game_observers->end(); iterator++) {
        (*iterator)->update(game_phase);
    }
}
