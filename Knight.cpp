#include "Knight.h"
#include "Helper.h"

namespace Chess
{
    bool Knight::legal_move_shape(const Position& start, const Position& end) const {

        // L-shaped movements:
        // The product of the number of tiles moved should equal 2
        // Since only 2*1 and 1*2 equals 2,
        // the sum of the deltas should equal 2
        // TODO: Check
        if(ABS((start.first - end.first) * (start.second - end.second)) == 2)
            return true;

        return false;
    }
}