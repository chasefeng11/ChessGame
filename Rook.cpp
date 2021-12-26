#include "Rook.h"
#include "Helper.h"

namespace Chess
{
    bool Rook::legal_move_shape(const Position& start, const Position& end) const {

        //Return false if rook moves diagonally
        if(start.first == end.first || start.second == end.second)
            return true;

        return false;
    }
}