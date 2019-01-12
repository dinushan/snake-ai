#ifndef AI_H
#define AI_H

#include <Game.h>
#include <cassert>
#include <iostream>

class AI
{
    public:
        AI();
        virtual ~AI();
        virtual game::Direction getNextMove(const game::Board& board) = 0;
        void dumpBoard() const;

    protected:

    private:

};

#endif // AI_H
