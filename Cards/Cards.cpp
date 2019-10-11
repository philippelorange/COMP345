#include <iostream>
#include "Cards.h"

// Beginning of Card functions
Card::Card(Type* type) {
    type_ = type;
}

string Card::get_type_name() const {
    string type_name[] = {"infantry", "cavalry", "artillery"};
    return type_name[*type_];
}

Card* Card::factory_card(Type* type_ptr, int type_number) {
    switch (type_number) {
        case 0: {
            *type_ptr = infantry;
            break;
        }
        case 1: {
            *type_ptr = cavalry;
            break;
        }
        case 2: {
            *type_ptr = artillery;
            break;
        }
        default: {
            *type_ptr = infantry;
        }
    }
    Card* card = new Card(type_ptr);
    return card;
}

//End of Card functions

//Beginning of Deck functions
Deck::Deck(vector<Card*>* cards) {
    deck_cards_ = cards;
}

void Deck::draw(Hand* hand) {
    if (hand->get_hand_cards()->size() > 4) {
        cout << "Can't draw, as there is too many cards in your hand" << endl;
        return;
    }

    int random_number = (int) random() % this->deck_cards_->size(); // get random number from size of cards
    hand->get_hand_cards()->push_back((*deck_cards_)[random_number]); // add card to hand
    this->deck_cards_->erase(this->deck_cards_->begin() + random_number);
}

vector<Card*>* Deck::get_deck_cards() const {
    return deck_cards_;
}

void Deck::print_card_in_deck() {
    cout << "\nCards still in deck \n" << endl;

    for (Card* deck_card: *this->get_deck_cards()) {
        cout << "Card " << deck_card->get_type_name() << " still in deck \n" << endl;
    }

    cout << "Size of deck " << this->get_deck_cards()->size() << endl;
}
//End of Deck functions

//Beginning of Hand functions
Hand::Hand() {
    hand_cards_ = new vector<Card*>;
}

vector<Card*>* Hand::get_hand_cards() const {
    return hand_cards_;
}

void Hand::delete_card_from_hand_and_add_it_to_deck(Deck* deck, int index) {
    deck->get_deck_cards()->push_back((*hand_cards_)[index]);
    this->hand_cards_->erase(this->hand_cards_->begin() + index);
}

void Hand::exchange(Deck* deck) {
    if (deck->get_deck_cards()->empty()) {
        cout << "Deck is empty! Cannot draw" << endl;
        return;
    }

    if (hand_cards_->size() < 3) {
        cout << "There is not enough cards in your hand to draw, you have " << hand_cards_->size() << " cards..."
             << endl;
        return;
    }

    int infantry_counter = 0, cavalry_counter = 0, artillery_counter = 0;

    for (Card* card : *hand_cards_) { // get number of each cards in your hand
        if (card->get_type_name() == "infantry")
            infantry_counter++;
        else if (card->get_type_name() == "cavalry")
            cavalry_counter++;
        else if (card->get_type_name() == "artillery")
            artillery_counter++;
    }

    if ((infantry_counter > 0 && cavalry_counter > 0 && artillery_counter > 0)) // Player has 1 of each
    {
        if (infantry_counter == 3 || cavalry_counter == 3 ||
            artillery_counter == 3) { // Player has 1 of each and 3 different card

            string answer;

            while (true) {
                cout << "You have 3 cards of the same type and 3 cards of type different, what do you want to do? "
                        "(same, different)" << endl;
                cin >> answer;
                if (answer == "same" || answer == "different") {
                    if (answer == "same") // player decide to exchange 3 same cards
                        this->find_and_exchange_which_card_is_highest(deck, infantry_counter, cavalry_counter);
                    else
                        this->exchange_three_different(deck); // player decide to exchange 3 different cards
                    break;
                } else
                    cout << "Enter a valid answer" << endl;
            }
        } else
            this->exchange_three_different(deck);

    } else if (infantry_counter > 2 || cavalry_counter > 2 || artillery_counter > 2) { // Player has 3 different
        this->find_and_exchange_which_card_is_highest(deck, infantry_counter, cavalry_counter);

    } else {
        cout << "Conditions aren't met for you to exchange cards..." << endl;
        return;
    }

    static int exchange_counter = 0; // static counter for armies
    exchange_counter++; // increment every time function get to this point
    cout << "Player receive " << exchange_counter * 5 << " armies " << endl;
}

void Hand::exchange_three_different(Deck* deck) {
    bool infantry_pushed = false, cavalry_pushed = false, artillery_pushed = false;

    for (int i = 0; i < hand_cards_->size();) { // erase-remove idiom
        if ((*hand_cards_)[i]->get_type_name() == "infantry" && !infantry_pushed) { // infantry part
            this->delete_card_from_hand_and_add_it_to_deck(deck, i);
            infantry_pushed = true;

        } else if ((*hand_cards_)[i]->get_type_name() == "cavalry" && !cavalry_pushed) { // cavalry part
            this->delete_card_from_hand_and_add_it_to_deck(deck, i);
            cavalry_pushed = true;

        } else if ((*hand_cards_)[i]->get_type_name() == "artillery" && !artillery_pushed) { // artillery part
            this->delete_card_from_hand_and_add_it_to_deck(deck, i);
            artillery_pushed = true;

        } else
            i++; // increment only if card hasn't been removed
    }
}

void Hand::exchange_three_same(Deck* deck, const string& type_of_card_position) {
    int card_pushed = 0;

    for (int i = 0; i < hand_cards_->size();) { // erase-remove idiom
        if ((*hand_cards_)[i]->get_type_name() == type_of_card_position && card_pushed != 3) {
            delete_card_from_hand_and_add_it_to_deck(deck, i);
            card_pushed++;
        } else
            i++; // increment only if card hasn't been removed
    }
}

void Hand::find_and_exchange_which_card_is_highest(Deck* deck, int infantry_counter, int cavalry_counter) {
    if (infantry_counter > 2) {
        exchange_three_same(deck, "infantry");

    } else if (cavalry_counter > 2) {
        exchange_three_same(deck, "cavalry");

    } else
        exchange_three_same(deck, "artillery");
}

void Hand::print_card_in_hand() {
    for (Card* hand_card: *this->get_hand_cards()) {
        cout << "Cards in hand: " << hand_card->get_type_name() << endl;
    }
}
//End of Hand functions