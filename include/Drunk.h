#ifndef DRUNK_H
#define DRUNK_H

#include <AI.h>


class Drunk : public AI
{
    public:
        Drunk();
        virtual ~Drunk();
        game::Direction getNextMove(const game::Board& board) override;

    protected:

    private:
        inline bool isOpposite(game::Direction first, game::Direction second) const;
};

#endif // DRUNK_H
