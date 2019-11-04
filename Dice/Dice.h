#pragma once

#include <vector>

//header class with all variables and functions
class Dice {
public:
    Dice();

    ~Dice();

    void Roll(int n);

    int getTotalDice() { return *total_dice; };

    void showHistory(int n);

    void showPercentage(int value);

    int* sortAsc(int** p, int n);

	static int* sortDsc(int n);


private:
    int* total_dice;
    std::vector<int*> history;
};