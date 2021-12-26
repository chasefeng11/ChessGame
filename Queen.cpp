#include "Queen.h"
#include "Helper.h"

namespace Chess
{
    bool Queen::legal_move_shape(const Position& start, const Position& end) const {

        //Check if queen moved either down/up a row/column
        if(start.first == end.first || start.second == end.second){
            return true;
        }

        //Checks to see if piece moved in a diagonal
        if(ABS(start.first - end.first) == ABS(start.second - end.second)){
            return true;
        }

        return false;
    }
}