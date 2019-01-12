#ifndef SIMPLEAI_H
#define SIMPLEAI_H

#include <AI.h>


class SimpleAI : public AI
{
    public:
        SimpleAI();
        virtual ~SimpleAI();
        game::Direction getNextMove(const game::Board& board) override;

    protected:

    private:
        bool canTurn(const game::Board& board, game::Direction direction) const;
};

#endif // SIMPLEAI_H
