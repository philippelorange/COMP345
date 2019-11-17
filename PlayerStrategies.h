#pragma once
#include <iostream>
using namespace std;
class Player;

class Strategy{
	public:
		virtual void fortify();
		virtual void attack();
		virtual void reinforce();
		virtual int* execute_strategy();
};

class AggressiveAI : public Strategy {
	public:
		int* execute_strategy();
};

class PassiveAI : public Strategy {
	public:
		int* execute_strategy();
};

class Human : public Strategy {
	public:
		int* execute_strategy();
};