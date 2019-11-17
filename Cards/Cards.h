#pragma once

#include <string>
#include <vector>

using namespace std;

class Deck;

class Hand;

enum Type {
    infantry, cavalry, artillery
};

class Card {
public:
    // constructor
    explicit Card(Type* type);

    // getters
    string get_type_name() const;

    // factory method
    static Card* factory_card(Type* type, int type_number);

private:
    // attributes
    Type* type_;

};

class Deck {
public:
    // constructor
    explicit Deck(vector<Card*>* cards);

    // getters
    vector<Card*>* get_deck_cards() const;

    // functions
    void draw(Hand* hand);

    void print_card_in_deck();

private:
    // attributes
    vector<Card*>* deck_cards_;
};

class Hand {
public:
    explicit Hand();

    // getters
    vector<Card*>* get_hand_cards() const;

    // functions

    void delete_card_from_hand_and_add_it_to_deck(Deck* deck, int index);

    int exchange(Deck* deck);

    void exchange_three_same(Deck* deck, const string& type_of_card_position);

    void exchange_three_different(Deck* deck);

    void find_and_exchange_which_card_is_highest(Deck* deck, int infantry_counter, int cavalry_counter);

    void print_card_in_hand();

private:
    // attributes
    vector<Card*>* hand_cards_;
};
