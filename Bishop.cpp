#include "Bishop.h"
#include "Helper.h"

namespace Chess
{
    bool Bishop::legal_move_shape(const Position& start, const Position& end) const {

        // Check if it can diagonally move
        // Check if the deltas are equal
        if(ABS(start.first - end.first) == ABS(start.second - end.second))
            return true;

        return false;
    }
}
