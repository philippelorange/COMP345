#include "Cards/Cards.h"
#include <iostream>

int main() {
    auto* cards = new vector<Card*>;
    srandom(time(nullptr)); // random seed to use unpredictable numbers

    cout << "Creating cards.." << endl;

    int nbr_cards = 42; // TODO: get number of cards from map instead of 42 (hardcoded)

    while (nbr_cards % 3 != 0) { // get number divisible by 3 for purpose of this assignment
        nbr_cards--;
    }

    for (int i = 0, type_number = 0; i < nbr_cards; i++, type_number++) { // create cards from map
        type_number == 3 ? type_number = 0 : type_number;
        Type* type_ptr = new Type;
        cards->push_back(Card::factory_card(type_ptr, type_number)); // add reference of card to vector
    }

    int* infantry_counter = new int(0);
    int* cavalry_counter = new int(0);
    int* artillery_counter = new int(0);

    for (Card* card: *cards) {
        cout << "Card " << card->get_type_name() << " created" << endl;
        if (card->get_type_name() == "infantry") // count how many cards in deck
            (*infantry_counter)++;
        else if (card->get_type_name() == "cavalry")
            (*cavalry_counter)++;
        else
            (*artillery_counter)++;
    }

    cout << "Successfully created " << *infantry_counter << " infantry cards" << endl;
    cout << "Successfully created " << *cavalry_counter << " cavalry cards" << endl;
    cout << "Successfully created " << *artillery_counter << " artillery cards" << endl;

    delete (infantry_counter); // delete unnecessary counter at this point
    delete (cavalry_counter);
    delete (artillery_counter);

    Deck* deck = new Deck(cards);
    Hand* hand = new Hand();
    string answer;

    while (true) { // loop until exit
        cout << "What do you want to do ? (draw, exchange, print, exit)" << endl;
        cin >> answer;

        if (answer == "draw") {
            cout << "Drawing a card.." << endl;
            deck->draw(hand);

        } else if (answer == "exchange") {
            cout << "Exchange cards.." << endl;
            hand->exchange(deck);

        } else if (answer == "print") {
            hand->print_card_in_hand();
            deck->print_card_in_deck();

        } else if (answer == "exit")
            break;
        else
            cout << "Enter a valid answer" << endl;
    }
}