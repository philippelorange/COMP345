#include "PlayerStrategies.h"

void Strategy::fortify()
{
}

void Strategy::attack()
{
}

void Strategy::reinforce()
{
}

int* Strategy::execute_strategy()
{
	return new int (0);
}



int* AggressiveAI::execute_strategy()
{
	return new int(1);
}


int* PassiveAI::execute_strategy()
{
	return new int(2);
}

int* Human::execute_strategy()
{
	return new int(3);
}
