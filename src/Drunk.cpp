#include "Drunk.h"
#include <cstdlib>

Drunk::Drunk()
{
    //ctor
}

Drunk::~Drunk()
{
    //dtor
}

//*************************************************************************************************
/*
    LEFT   = 1  001 b
    UP     = 3  010 b
    RIGHT  = 2  011 b
    DOWN   = 4  100 b
*/
bool Drunk::isOpposite(game::Direction first, game::Direction second) const
{
    int f = static_cast<int>(first);
    int s = static_cast<int>(second);
    return abs(f -s) == 2;

}

//*************************************************************************************************
game::Direction Drunk::getNextMove(const game::Board& board)
{
    game::Direction dir = static_cast<game::Direction>(rand() % 5);
    if (dir == game::Direction::UNDEFINED || dir == board.m_direction)
        return board.m_direction;

    if (isOpposite(dir, board.m_direction ))
        return board.m_direction;

    return dir;
}
