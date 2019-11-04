#include "Dice.h"
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace std;


//constructor
Dice::Dice() {
    total_dice = new int(0);
}

//destructor
Dice::~Dice() {
    delete total_dice;
}

//roll function with parameter n to find # of times to roll
void Dice::Roll(int n) {
    //if roll once
    if (n == 1) {
        //pointer with value of random from 1-6
        int* val1 = new int(rand() % 6 + 1);
        //print value
        cout << *val1 << endl;
        //push value into vector
        history.push_back(val1);

        (*total_dice)++;
    }
        //if roll twice
    else if (n == 2) {
        //same logic as when n==1 but done 2x
        int* val1 = new int(rand() % 6 + 1);
        int* val2 = new int(rand() % 6 + 1);

        if (*val1 > *val2) {
            int* t = val2;
            val2 = val1;
            val1 = t;
        }

        cout << *val1 << ' ';
        cout << *val2 << endl;

        history.push_back(val1);
        history.push_back(val2);

        (*total_dice) += 2;
    } else if (n == 3) {
        //same logic as when n==1 but done 3x
        int* val1 = new int(rand() % 6 + 1);
        int* val2 = new int(rand() % 6 + 1);
        int* val3 = new int(rand() % 6 + 1);

        if (*val1 > *val2) {
            int* t = val2;
            val2 = val1;
            val1 = t;
        }

        if (*val2 > *val3) {
            int* t = val3;
            val3 = val2;
            val2 = t;
        }

        if (*val1 > *val2) {
            int* t = val2;
            val2 = val1;
            val1 = t;
        }

        cout << *val1 << ' ';
        cout << *val2 << ' ';
        cout << *val3 << endl;

        history.push_back(val1);
        history.push_back(val2);
        history.push_back(val3);

        (*total_dice) += 3;
    } else {
        //detects if number entered is wrong
        cout << "Please enter a number between 1 and 3" << endl;
    }
}

//show past rolls
void Dice::showHistory(int n) {
    std::cout << "Dice " << n << " history: ";
    //iterates over all elements in vector
    for (int* i = new int(0); *i < history.size(); (*i)++)
        std::cout << *history.at(*i) << ' '; //prints all values in vector
    //newline for formatting
    std::cout << endl;
}

void Dice::showPercentage(int value) {
    //find size of vector (# of total rolls)
    double* num_rolls = new double(history.size());
    double* counter = new double(0);
    for (int* i = new int(0); *i < history.size(); (*i)++) {
        //count number of times value we want to see shows up
        if (*history.at(*i) == value) {
            (*counter)++;
        }
    }
    //detects if value showed up 0 times
    if (std::isnan((*counter) / (*num_rolls))) {
        cout << "The number " << value << " rolled " << "0% of times" << endl;
    } else {
        //prints percentages
        cout << "The number " << value << " rolled " << (*counter) / (*num_rolls) * 100 << "% of times" << endl;
    }
	static int* Dice::sortDsc(int n) {
		static int  sorted_dices[n];
		if (n == 1) {
			//pointer with value of random from 1-6
			sorted_dices[0] = rand() % 6 + 1;
		}
		//if roll twice
		else if (n == 2) {
			//same logic as when n==1 but done 2x
			int val1 = rand() % 6 + 1;
			int val2 = rand() % 6 + 1;

			if (val1 < val2) {
				int t = val1;
				val1 = val2;
				val2 = t;
			}

			sorted_dices[0] = val1;
			sorted_dices[1] = val2;
		}
		else if (n == 3) {
			//same logic as when n==1 but done 3x
			int val1 = rand() % 6 + 1;
			int val2 = rand() % 6 + 1;
			int val3 = rand() % 6 + 1;

			if (val1 < val2) {
				int t = val1;
				val1 = val2;
				val2 = t;
			}

			if (val2 < val3) {
				int t = val2;
				val2 = val3;
				val3 = t;
			}

			if (val1 < val2) {
				int t = val1;
				val1 = val2;
				val2 = t;
			}
			sorted_dices[0] = val1;
			sorted_dices[1] = val2;
			sorted_dices[2] = val3;
		};
		return sorted_dices;
}

