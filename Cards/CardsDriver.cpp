#include "Cards/Cards.h"
#include <iostream>
#include <Map.h>

int main() {
    //[continents]
    auto* continent1 = new Continent("Maritimes");
    auto* continent2 = new Continent("Quebec");
    auto* continent3 = new Continent("Ontario");

    auto* continents = new vector<Continent*>;
    continents->push_back(continent1);
    continents->push_back(continent2);
    continents->push_back(continent3);

    //[countries]
    auto* c1 = new Country("NB", continent1);
    auto* c2 = new Country("PEI", continent1);
    auto* c3 = new Country("NS", continent1);
    auto* c4 = new Country("Newfoundland", continent1);
    auto* c5 = new Country("Labrador", continent1);

    auto* c6 = new Country("QC-North", continent2);
    auto* c7 = new Country("QC-Central", continent2);
    auto* c8 = new Country("QC-South", continent2);

    auto* c9 = new Country("Ottawa", continent3);

    auto* countries = new vector<Country*>;
    countries->push_back(c1);
    countries->push_back(c2);
    countries->push_back(c3);
    countries->push_back(c4);
    countries->push_back(c5);
    countries->push_back(c6);
    countries->push_back(c7);
    countries->push_back(c8);
    countries->push_back(c9);

    continent1->add_country(c1);
    continent1->add_country(c2);
    continent1->add_country(c3);
    continent1->add_country(c4);
    continent1->add_country(c5);
    //continent1->add_country(c6); //Test case 3: Uncomment this. (Adds a country to a second continent)

    continent2->add_country(c6);
    continent2->add_country(c7);
    continent2->add_country(c8);

    continent3->add_country(c9);

    //[borders]
    c1->add_adjacent_country(c2);
    c1->add_adjacent_country(c3);
    c1->add_adjacent_country(c8);

    c2->add_adjacent_country(c1);
    c2->add_adjacent_country(c3);

    c3->add_adjacent_country(c1);
    c3->add_adjacent_country(c2);
    c3->add_adjacent_country(c4);

    c4->add_adjacent_country(c3);
    c4->add_adjacent_country(c5);
    c4->add_adjacent_country(c7);

    c5->add_adjacent_country(c4);
    c5->add_adjacent_country(c6);
    c5->add_adjacent_country(c7);

    c6->add_adjacent_country(c5);
    c6->add_adjacent_country(c7); //Test case 2: Comment this out. (Breaks QC-North off of the rest of Quebec, forces you to pass by Maritimes, invalid)

    c7->add_adjacent_country(c4);
    c7->add_adjacent_country(c5);
    c7->add_adjacent_country(c6); //Test case 2: Comment this out. (Breaks QC-North off of the rest of Quebec, forces you to pass by Maritimes, invalid)
    c7->add_adjacent_country(c8);

    c8->add_adjacent_country(c7);
    c8->add_adjacent_country(c9); //Test case 1: Comment this out. (Breaks Ontario off of Quebec)

    c9->add_adjacent_country(c8); //Test case 1: Comment this out. (Breaks Ontario off of Quebec)

    //Create map
    Map* map = new Map(continents, countries);


    auto* cards = new vector<Card*>;
    srandom(time(nullptr)); // random seed to use unpredictable numbers

    cout << "Creating cards.." << endl;

    int nbr_cards = map->get_countries()->size();

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