#include "Game.h"

int main(int argc, char** argv)
{
    game::Game game;
    if (game.init() == false)
    {
        printf("Error initializing game");
        return -1;
    }

    game.start();

    return 0;
}
