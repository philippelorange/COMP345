#include <iostream>
#include "GameObservers.h"
#include "Player/Player.h"
#include <algorithm>

// GameObservers
PhaseObserver::~PhaseObserver() = default;

PhaseObserver::PhaseObserver() = default;

StatisticObserver::~StatisticObserver() = default;

StatisticObserver::StatisticObserver() = default;

//Concrete observers

ConcretePhaseObserver::ConcretePhaseObserver(Player* player) {
    this->player = player;
}

void ConcretePhaseObserver::display_game_phase(GamePhase game_phase) {
    string current_phase = get_phase_string(game_phase);
    cout << this->player->get_player_name() << " phase: " << current_phase << endl;
}

void ConcretePhaseObserver::display_relevant_info(GamePhase game_phase) {
    string current_phase = get_phase_string(game_phase);
    cout << current_phase << " relevant info:" << endl;
}

void ConcretePhaseObserver::update(GamePhase game_phase) {
    this->display_game_phase(game_phase);
    this->display_relevant_info(game_phase);
}

string ConcretePhaseObserver::get_phase_string(GamePhase game_phase) {
    string type_name[] = {"reinforcement", "attack", "fortify"};
    return type_name[game_phase];
}

ConcreteStatisticObserver::ConcreteStatisticObserver(Map* map) {
    this->map = map;
}

void ConcreteStatisticObserver::update() {
    this->display();
}

void ConcreteStatisticObserver::display() {

    if (this->map->player_has_been_set()) {
        cout << "***** Statistic Time *****" << endl;

        vector<Player*> players;

        for (int i = 0; i < this->map->get_countries()->size(); i++) {
            if (i == 0) {
                players.push_back(
                        this->map->get_countries()->at(i)->get_player()); // add first player
            } else {
                for (int j = 0; j < players.size(); j++) { // check if player is inside players vector
                    if (players[j]->get_player_name() ==
                        this->map->get_countries()->at((i))->get_player()->get_player_name()) {
                        break;
                    }
                    if (j + 1 >= players.size()) { // end of second loop
                        players.push_back(this->map->get_countries()->at(i)->get_player());
                        break;
                    }
                }
            }
        }

        if (players.size() == 1) {
            cout << "CONGRATS TO " << players[0]->get_player_name() << " FOR WINNING THE GAME !!" << endl;
        } else {
            for (Player* player : players) {
                double percentage =
                        (double) player->get_player_owned_countries()->size() / (double) map->get_countries()->size();
                cout << "Player " << player->get_player_name() << " owns " << percentage << " of all countries ! (" <<
                     player->get_player_owned_countries()->size() << "/" << map->get_countries()->size() << ")" << endl;
            }
        }
    }

}

// Subject

PhaseObservable::PhaseObservable() {
    this->game_observers = new list<PhaseObserver*>;
};

PhaseObservable::~PhaseObservable() {
    delete game_observers;
};

void PhaseObservable::attach(PhaseObserver* observer) {
    this->game_observers->push_back(observer);
}

void PhaseObservable::detach(PhaseObserver* observer) {
    this->game_observers->remove(observer);
}

void PhaseObservable::notify(GamePhase game_phase) {
    auto iterator = this->game_observers->begin();
    for (; iterator != game_observers->end(); ++iterator) {
        (*iterator)->update(game_phase);
    }
}

StatisticObservable::StatisticObservable() {
    this->game_stats_observers = new list<StatisticObserver*>;
}

StatisticObservable::~StatisticObservable() {
    delete game_stats_observers;
}

void StatisticObservable::detach(StatisticObserver* observer) {
    this->game_stats_observers->remove(observer);
}

void StatisticObservable::notify() {
    for (StatisticObserver* statistic_observer : *game_stats_observers) {
        statistic_observer->update();
    }
}

void StatisticObservable::attach(StatisticObserver* observer) {
    this->game_stats_observers->push_back(observer);
}
