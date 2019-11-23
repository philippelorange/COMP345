#pragma once
#include <iostream>
#include <Map/Map.h>

using namespace std;
class Player;

class Strategy{
public:
    virtual void place_army(vector<Country*>* countries) = 0;

    virtual Country* get_country_to_reinforce(vector<Country*>* countries) = 0;

    virtual bool should_fortify() = 0;
    virtual Country* get_country_to_fortify(vector<Country*>* countries) = 0;
    virtual Country* get_fortification_source(Country* destination) = 0;
    virtual int get_fortification_armies(Country* source) = 0;

    virtual bool should_attack() = 0;
    virtual Country* get_country_to_attack_from(vector<Country*>* countries) = 0;
    virtual Country* get_country_to_attack(vector<Country*>* countries) = 0;
    virtual int get_attack_dice(int max_dice) = 0;
    virtual int get_defend_dice(int max_dice) = 0;
    virtual int get_armies_to_move(int max) = 0;

};

class AggressiveStrategy : public Strategy {
public:
    void place_army(vector<Country*>* countries) override;

    Country* get_country_to_reinforce(vector<Country*>* countries) override;

    bool should_fortify() override;
    Country* get_country_to_fortify(vector<Country*>* countries) override;
    Country* get_fortification_source(Country* destination) override;
    int get_fortification_armies(Country* source) override;

    bool should_attack() override;
    Country* get_country_to_attack_from(vector<Country*>* countries) override;
    Country* get_country_to_attack(vector<Country*>* countries) override;
    int get_attack_dice(int max_dice) override;
    int get_defend_dice(int max_dice) override;
    int get_armies_to_move(int max) override;
};

class BenevolentStrategy : public Strategy {
public:
    void place_army(vector<Country*>* countries) override;

    Country* get_country_to_reinforce(vector<Country*>* countries) override;

    bool should_fortify() override;
    Country* get_country_to_fortify(vector<Country*>* countries) override;
    Country* get_fortification_source(Country* destination) override;
    int get_fortification_armies(Country* source) override;

    bool should_attack() override;
    Country* get_country_to_attack_from(vector<Country*>* countries) override;
    Country* get_country_to_attack(vector<Country*>* countries) override;
    int get_attack_dice(int max_dice) override;
    int get_defend_dice(int max_dice) override;
    int get_armies_to_move(int max) override;
};

class HumanStrategy : public Strategy {
public:
    void place_army(vector<Country*>* countries) override;

    Country* get_country_to_reinforce(vector<Country*>* countries) override;

    bool should_fortify() override;
    Country* get_country_to_fortify(vector<Country*>* countries) override;
    Country* get_fortification_source(Country* destination) override;
    int get_fortification_armies(Country* source) override;

    bool should_attack() override;
    Country* get_country_to_attack_from(vector<Country*>* countries) override;
    Country* get_country_to_attack(vector<Country*>* countries) override;
    int get_attack_dice(int max_dice) override;
    int get_defend_dice(int max_dice) override;
    int get_armies_to_move(int max) override;
};