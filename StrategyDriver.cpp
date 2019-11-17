#include <iostream>
#include "Player/Player.h"

using namespace std;

int main() {
	Strategy* AI = new AggressiveAI();
	Strategy* pAI = new PassiveAI();

	
	Player* p = new Player("Player 1", NULL);
	//p->setStrategy(AI);
	p->get_strategy()->fortify();
	//p->setStrategy(pAI);
	p->get_strategy()->attack();
}