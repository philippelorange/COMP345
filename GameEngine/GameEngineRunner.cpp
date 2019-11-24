#include "GameEngine.h"
#include <iostream>

using namespace std;

int main() {
    auto* gameEngine = new GameEngine();
    gameEngine->start();
}