#include "Pawn.h"
#include "Helper.h"

namespace Chess
{
    bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
        int range = 1;

        if (is_white()){
            // Checks if pawn is in starting row
            if (start.second == '2')
                range = 2;

            // Checks if end location is in row range
            if (end.second - start.second > range || end.second - start.second < 0)
                return false;

            // Checks if end location is in same column
            if (end.first != start.first)
                return false;
        }
        // Repeats for black pieces, going down in board
        else {
            if (start.second == '7')
                range = 2;

            if (start.second - end.second > range || start.second - end.second < 0)
                return false;

            if (end.first != start.first)
                return false;
        }

        return true;
    }

    bool Pawn::legal_capture_shape(const Position &start, const Position &end) const {

        // Forward Diagonal if white
        if(is_white()){
            if(ABS(end.first - start.first) == 1 && end.second - start.second == 1)
                return true;
        }else{
            if(ABS(end.first - start.first) == 1 && start.second - end.second == 1)
                return true;
        }

        return false;
    }
}
