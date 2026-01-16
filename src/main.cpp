#include "Game.h"

int main(int, char const **)
{
    Game &game = Game::getInstance();
    game.init();
    game.run();

    return 0;
}