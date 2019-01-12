#include "HamiltonianCircuitAI.h"

using Direction = game::Direction;

HamiltonianCircuitAI::HamiltonianCircuitAI(): AI::AI()
{
    //ctor
}

HamiltonianCircuitAI::~HamiltonianCircuitAI()
{
    //dtor
}

//*************************************************************************************************
game::Direction HamiltonianCircuitAI::getNextMove(const game::Board& board)
{
    int X = board.m_x[0];
    int Y = board.m_y[0];
    if (X == 1)
    {
        if (Y == (BOARD_HEIGT - 1))
            return Direction::LEFT;
        if (Y == 0)
            return Direction::RIGHT;
        if (board.m_direction == Direction::LEFT)
            return Direction::DOWN;
        if (board.m_direction == Direction::DOWN)
            return Direction::RIGHT;
    }
    else if (X >= 1 && X <= (BOARD_WIDTH - 2))
    {
        if (board.m_direction == Direction::RIGHT || board.m_direction == Direction::LEFT)
            return board.m_direction;
    }
    else if (X == (BOARD_WIDTH - 1))
    {
        if (board.m_direction == Direction::DOWN)
            return Direction::LEFT;
        if (board.m_direction == Direction::RIGHT)
            return Direction::DOWN;
    }
    else if (X == 0)
    {
        if (Y != 0)
            return Direction::UP;
        else
            return Direction::RIGHT;
    }
    return board.m_direction;
}
