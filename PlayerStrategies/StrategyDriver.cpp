#include <iostream>
#include "Player/Player.h"

using namespace std;

int main() {
	Strategy* AI = new AggressiveAI();
	Strategy* pAI = new PassiveAI();

	
	Player* p = new Player("Player 1", NULL);
	p->setStrategy(new Human());
	p->fortify();
}