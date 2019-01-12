#ifndef A_STARSEARCH_H
#define A_STARSEARCH_H

#include <AI.h>

class HamiltonianCircuitAI : public AI
{
    public:
        HamiltonianCircuitAI();
        virtual ~HamiltonianCircuitAI();
        game::Direction getNextMove(const game::Board& board) override;

    protected:

    private:


};

#endif // A_STARSEARCH_H
