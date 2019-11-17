#pragma once
#include <iostream>
using namespace std;
class Player;

class Strategy{
	public:
		virtual void fortify();
		virtual void attack();
		virtual void reinforce();
};

class AggressiveAI : public Strategy {
	public:
		void fortify();
		void attack();
		void reinforce();
};

class PassiveAI : public Strategy {
	public:
		void fortify();
		void attack();
		void reinforce();
};

class Human : public Strategy {
	public:
		void fortify();
		void attack();
		void reinforce();
};