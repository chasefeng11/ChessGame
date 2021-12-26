#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
    class Board {

        // Throughout, we will be accessing board positions using Position defined in Piece.h.
        // The assumption is that the first value is the column with values in
        // {'A','B','C','D','E','F','G','H'} (all caps)
        // and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

    public:
        // Default constructor
        Board();

        // Copy constructor
        Board(const Board &b);

        // Overload assignment operator 
        Board& operator=(const Board &b);

        // Destructor
        ~Board();

        // Returns a const pointer to the piece at a prescribed location if it exists,
        // or nullptr if there is nothing there.
        const Piece* operator() (const Position& position) const;

        // Returns the position of the requested piece
        Position find_by_piece(const char& piece_designator) const;

        // Attempts to add a new piece with the specified designator, at the given position.
        // Throw exception for the following cases:
        // -- the designator is invalid, throw exception with error message "invalid designator"
        // -- the specified position is not on the board, throw exception with error message "invalid position"
        // -- if the specified position is occupied, throw exception with error message "position is occupied"
        void add_piece(const Position& position, const char& piece_designator);

        // Removes a piece object at the given position.
        void remove_piece(const Position& position);

        // Remove all pieces in a board
        void remove_all();

        // Displays the board by printing it to stdout
        void display() const;

        // Returns true if the board has the right number of kings on it
        bool has_valid_kings() const;

        // Cleanup function for removing any allocated memory
        void cleanup();

        // Map to store the locations and types of each piece
        typedef std::map<Position, Piece*> BoardType;

        // Constant Iterator for the Board object
        // Keeps track of a cell location at all times, and moves up/right
        class const_iterator {
        public:
            const_iterator(Position initial_pos) : pos(initial_pos) {};

            // Moves pointer to next cell
            const_iterator &operator++() {
                char cur_col = pos.first;
                int cur_row = pos.second;

                // Makes sure we don't move off the board when we get to end of row
                if (cur_col == 'H') {
                    // Refers to cell out of bounds of iterator (i.e. .end());
                    if (cur_row == '8') {
                        pos = Position(-1, -1);
                    } else {
                        pos = Position('A', cur_row + 1);
                    }
                // Next position to visit when traversing as normal (not end of row)
                } else {
                    pos = Position(cur_col + 1, cur_row);
                }
                
                return *this;
            }

            // Overload == operator to check if two iterators point to the same location
            bool operator==(const const_iterator &o) const { 
                return (pos.first == o.pos.first && pos.second == o.pos.second);
            }

            // Checks if two iterators don't point to the same location
            bool operator!=(const const_iterator &o) const { return !(*this == o); }

            // Returns position the iterator is currently on
            Position current_pos() { return pos; }

        private:
            Position pos;
        };

        // Returns an iterator pointing to "one-past" the last cell in the board
        const_iterator cend() const {
            return const_iterator(Position(-1, -1));
        }

        // Returns an iterator pointing to first cell in the board (bottom-left)
        const_iterator cbegin() const {
            return const_iterator(Position('A', '1'));
        }


    private:
        // The sparse map storing the pieces, keyed off locations
        std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);
    };
}
#endif // BOARD_H
