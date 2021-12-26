#include "King.h"
#include "Helper.h"

namespace Chess {
    bool King::legal_move_shape(const Position& start, const Position& end) const {
        // Check if it can move in all one direction
        if (ABS(start.first - end.first) < 2 && ABS(start.second - end.second) < 2) {
            return true;
        }
        
        return false;
    }
}