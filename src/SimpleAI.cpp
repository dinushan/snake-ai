#include "SimpleAI.h"

using Direction = game::Direction;

SimpleAI::SimpleAI()
{
    //ctor
}

SimpleAI::~SimpleAI()
{
    //dtor
}

//*************************************************************************************************
game::Direction SimpleAI::getNextMove(const game::Board& board)
{
    if (board.m_x[0] < board.m_GoalX && board.m_direction != Direction::LEFT)
        return Direction::RIGHT;
    if (board.m_x[0] > board.m_GoalX && board.m_direction != Direction::RIGHT)
        return Direction::LEFT;
    if (board.m_x[0] == board.m_GoalX)
    {
        if (board.m_y[0] < board.m_GoalY && board.m_direction != Direction::UP)
            return Direction::DOWN;
        if (board.m_y[0] < board.m_GoalY && board.m_direction == Direction::UP)
        {
            if (canTurn(board, Direction::LEFT)) return Direction::LEFT;
            if (canTurn(board, Direction::RIGHT)) return Direction::RIGHT;
            return board.m_direction;
        }
        if (board.m_y[0] > board.m_GoalY && board.m_direction != Direction::DOWN)
            return Direction::UP;
        if (board.m_y[0] > board.m_GoalY && board.m_direction == Direction::DOWN)
        {
            if (canTurn(board, Direction::LEFT)) return Direction::LEFT;
            if (canTurn(board, Direction::RIGHT)) return Direction::RIGHT;
            return board.m_direction;
        }
    }

    if (board.m_y[0] < board.m_GoalY && board.m_direction != Direction::UP)
        return Direction::DOWN;
    if (board.m_y[0] > board.m_GoalY && board.m_direction != Direction::DOWN)
        return Direction::UP;
    if (board.m_y[0] == board.m_GoalY)
    {
        if (board.m_x[0] < board.m_GoalX && board.m_direction != Direction::RIGHT)
            return Direction::RIGHT;
        if (board.m_x[0] < board.m_GoalX && board.m_direction == Direction::RIGHT)
        {
            if (canTurn(board, Direction::UP)) return Direction::UP;
            if (canTurn(board, Direction::DOWN)) return Direction::DOWN;
            return board.m_direction;
        }
        if (board.m_x[0] > board.m_GoalX && board.m_direction != Direction::LEFT)
            return Direction::UP;
        if (board.m_x[0] > board.m_GoalX && board.m_direction == Direction::LEFT)
        {
            if (canTurn(board, Direction::UP)) return Direction::UP;
            if (canTurn(board, Direction::DOWN)) return Direction::DOWN;
            return board.m_direction;
        }
    }

    return board.m_direction;
}

//*************************************************************************************************
bool SimpleAI::canTurn(const game::Board& board, Direction direction) const
{
    switch (direction)
    {
        case Direction::LEFT:
            if ((board.m_x[0] - 1) == 0)
                return false;
            for (int i = 1; i < board.m_length; ++i)
            {
                if ((board.m_x[0] - 1) == board.m_x[i] && board.m_y[0] == board.m_y[i])
                    return false;
            }
            return true;
        case Direction::RIGHT:
            if ((board.m_x[0] + 1) == BOARD_WIDTH)
                return false;
            for (int i = 1; i < board.m_length; ++i)
            {
                if ((board.m_x[0] + 1) == board.m_x[i] && board.m_y[0] == board.m_y[i])
                    return false;
            }
            return true;
        case Direction::UP:
            if ((board.m_y[0] - 1) == 0)
                return false;
            for (int i = 1; i < board.m_length; ++i)
            {
                if ((board.m_y[0] - 1) == board.m_y[i] && board.m_x[0] == board.m_x[i])
                    return false;
            }
            return true;
        case Direction::DOWN:
            if ((board.m_y[0] + 1) == BOARD_WIDTH)
                return false;
            for (int i = 1; i < board.m_length; ++i)
            {
                if ((board.m_y[0] + 1) == board.m_y[i] && board.m_x[0] == board.m_x[i])
                    return false;
            }
            return true;
    }

    return false;
}

