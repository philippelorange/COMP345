#include "Dice.h"
#include <iostream>
#include <time.h>
using namespace std;

int main() {

	//variable initialization 
	Dice* dice1 = new Dice;
	Dice* dice2 = new Dice;
	int* num_dice = new int;
	int* percent_value = new int;
	string* command = new string;

	//initialize random seed
	srand((unsigned)time(NULL));

	//loops as long as someone doesnt exit
	while (true) {
		//player prompt
		cout << "Player 1: What would you like to do? (roll, history, percentage, exit) **CASE SENSITIVE" << endl;
		cin >> *command;
		//check to see what player writes
		if ((*command).compare("roll") == 0) {
			cout << "How many dice would you like to roll? (1-3)" << endl;
			cin >> *num_dice;
			//function call roll
			dice1->Roll(*num_dice);
		}
		else if ((*command).compare("history") == 0) {
			//function call display past rolls with player # as argument
			dice1->showHistory(1);
		}
		else if ((*command).compare("percentage") == 0) {
			//prompt for # to find percentage of times it's been rolled
			cout << "What value would you like to find the percentage of? (1-6)" << endl;
			cin >> *percent_value;
			//turns arent properly handled, catches if player entered wrong number
			if (*percent_value < 1 || *percent_value > 6) {
				cout << "Please enter a valid value, turn skipped" << endl;
			}
			else {
				//display history
				dice1->showPercentage(*percent_value);
			}

		}
		//stops program when exited
		else if ((*command).compare("exit") == 0) {
			return 0;
		}
		else {
			//catches if player enters wrong command
			cout << "Command not recognized, please try again" << endl;
		}

		//same logic as player 1
		cout << "Player 2: What would you like to do? (roll, history, percentage, exit) **CASE SENSITIVE" << endl;
		cin >> *command;
		if ((*command).compare("roll") == 0) {
			cout << "How many dice would you like to roll?" << endl;
			cin >> *num_dice;
			dice2->Roll(*num_dice);
		}
		else if ((*command).compare("history") == 0) {
			dice2->showHistory(2);
		}
		else if ((*command).compare("percentage") == 0) {
			cout << "What value would you like to find the percentage of? (1-6)" << endl;
			cin >> *percent_value;
			if (*percent_value < 1 || *percent_value > 6) {
				cout << "Please enter a valid value, turn skipped" << endl;
			}
			else {
				dice2->showPercentage(*percent_value);
			}

		}
		else if ((*command).compare("exit") == 0) {
			return 0;
		}
		else {
			cout << "Command not recognized, please try again" << endl;
		}
	}
} 