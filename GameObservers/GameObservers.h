#pragma once

#include <list>

class Player;

class Map;

using namespace std;

enum GamePhase {
    reinforcement, attack, fortify
};

// Observers

class PhaseObserver {
public:
    ~PhaseObserver();

    virtual void update(GamePhase game_phase) = 0;

protected:
    PhaseObserver();
};

class StatisticObserver {
public:
    ~StatisticObserver();

    virtual void update() = 0;

protected:
    StatisticObserver();
};

// Concrete observers

class ConcretePhaseObserver : public PhaseObserver {
public:

    explicit ConcretePhaseObserver(Player* player);

    void update(GamePhase game_phase) override;

private:

    Player* player;

    void display_game_phase(GamePhase game_phase);

    void display_relevant_info(GamePhase game_phase);

    string get_phase_string(GamePhase game_phase);
};

class ConcreteStatisticObserver : public StatisticObserver {
public:

    explicit ConcreteStatisticObserver(Map* map);

    void update() override;

private:
    Map* map;

    void display();
};

// Subjects

class PhaseObservable {
public:
    PhaseObservable();

    ~PhaseObservable();

    virtual void detach(PhaseObserver* observer);

    virtual void notify(GamePhase game_phase);

    virtual void attach(PhaseObserver* observer);

private:
    list<PhaseObserver*>* game_observers;

};

class StatisticObservable {
public:
    StatisticObservable();

    ~StatisticObservable();

    virtual void detach(StatisticObserver* observer);

    virtual void notify();

    virtual void attach(StatisticObserver* observer);

private:
    list<StatisticObserver*>* game_stats_observers;

};

