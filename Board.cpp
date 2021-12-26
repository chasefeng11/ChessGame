#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess {
    Board::Board() {}

    // Copy constructor
    Board::Board(const Board &board) {
        *this = board;
    }

    // Assignment Operator
    Board& Board::operator=(const Board & board) {
        cleanup();
        for (const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            Position pos = cit.current_pos();
            const Piece* piece = board(pos);
            if (piece != nullptr){
                add_piece(pos, piece->to_ascii());
            }
        }
        return *this;
    }

    // Destructor
    Board::~Board() {
        cleanup();
    }

    const Piece* Board::operator()(const Position &position) const {
        for (BoardType::const_iterator cit = occ.cbegin(); cit != occ.cend(); ++cit) {
            if (cit->first == position) {
                return cit->second;
            }
        }
        return nullptr;
    }


    Position Board::find_by_piece(const char &piece_designator) const {
        for (BoardType::const_iterator cit = occ.cbegin(); cit != occ.cend(); ++cit) {
            if ((cit->second)->to_ascii() == piece_designator)
                return cit->first;
        }
        return std::make_pair(0, 0);
    }

    // Adds new piece to game board
    void Board::add_piece(const Position &position, const char &piece_designator) {
        // Creates piece object
        Piece *piece = create_piece(piece_designator);

        // Piece type is not valid
        if (piece == nullptr) {
            throw Exception("invalid designator");
        }

        const Piece *existing_piece = (*this)(position);

        // Something exists at that position
        if (existing_piece != nullptr) {
            throw Exception("position is occupied");
        }

        // Invalid position
        if (position.first > 'H' || position.first < 'A' || position.second < '1' || position.second > '8') {
            throw Exception("invalid position");
        }

        occ[position] = piece;
    }

    // Removes piece from game board
    void Board::remove_piece(const Position &position) {
        // Frees allocated memory
        const Piece *piece = (*this)(position);
        delete piece;
        occ.erase(position);
    }

    // Removes all pieces of the board
    void Board::remove_all() {
        for (BoardType::const_iterator it = occ.cbegin(); it != occ.cend(); ++it) {
            const Piece* piece = (*this)(it->first);
            delete piece;
        }
        occ.clear();
    }

    // Displays chess board in console while user plays game
    void Board::display() const {
        // Empty space for top left
        std::cout << "  ";

        // Column markings
        for (char it = 'A'; it < 'I'; it++) {
            std::cout << it << " ";
        }

        std::cout << std::endl;

        for (int row = 8; row > 0; row--) {
            // Row markings
            std::cout << row << " ";

            for (int col = 0; col < 8; col++) {
                // Add 48 so that it is the number 0 on the ascii table for the smallest
                // Add 16 for the first so that 1 becomes A, 2 becomes B, etc. etc.
                // Finally, 1 is added since 0th column doesn't exist
                Position cur = Position(col + 16 + 48 + 1, row + 48);

                // Check if any pieces exist in the position
                const Piece *piece = (*this)(cur);

                if (piece == nullptr) {
                    // Not found
                    std::cout << "x ";
                } else{
                    char ascii = piece->to_ascii();

                    // Check if the letter is capital (white)
                    // If it is, color it red
                    // If it isn't color it blue
                    if (ascii >= 'A' && ascii <= 'Z') {
                        Terminal::color_fg(true, Terminal::RED);
                    } else {
                        Terminal::color_fg(true, Terminal::BLUE);
                    }

                    std::cout << ascii << " ";

                    Terminal::set_default();
                }
            }

            std::cout << std::endl;
        }
    }

    bool Board::has_valid_kings() const {
        int white_king_count = 0;
        int black_king_count = 0;
        for (std::map<std::pair<char, char>, Piece *>::const_iterator it = occ.begin(); it != occ.end(); it++) {
            if (it->second) {
                switch (it->second->to_ascii()) {
                case 'K':
                    white_king_count++;
                    break;
                case 'k':
                    black_king_count++;
                    break;
                }
            }
        }
        return (white_king_count == 1) && (black_king_count == 1);
    }

    // Deallocate all the board pieces - for destructor & cleanup use
    void Board::cleanup() {
        for (BoardType::iterator it = occ.begin(); it != occ.end(); ++it) {
            delete it->second;
        }
    }

    std::ostream &operator<<(std::ostream &os, const Board &board) {
        for (char r = '8'; r >= '1'; r--) {
            for (char c = 'A'; c <= 'H'; c++) {
                const Piece *piece = board(Position(c, r));
                if (piece) {
                    os << piece->to_ascii();
                } else {
                    os << '-';
                }
            }
            os << std::endl;
        }
        return os;
    }
}
