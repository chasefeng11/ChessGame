#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess {

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();

		// Copy constructor
		Game(const Game &g);

		// Assignment operator
		Game& operator=(const Game &game);

		// Returns true if it is white's turn
		bool turn_white() const { return is_white_turn; }
    
        	// Displays the game by printing it to stdout
		void display() const { board.display(); }
    
        	// Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);

		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Test if the path is clear to the destination
		bool is_path_clear(const Position& start, const Position& end) const;

		// Is the path linear (diagonal, horizontal, vertical)
		bool is_path_linear(const Position& start, const Position& end) const;

		// Sees if a move will result in check
		bool would_check(const Position &start, const Position &end) const;

		// determines if a piece is elligible to be promoted
		bool check_promotion(const Position& start, const Position& end) const;

		// Determines if a move is legal
		bool is_possible_move(const Position& pos) const;

		// Tries to see if a player can move to allow king to escape from check
		bool prevent_check(const Position& pos) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_stalemate(const bool& white) const;

        	// Returns the total material point value of the designated player
        	int point_value(const bool& white) const;

		// Destructor
		~Game();

        	// Run cleanup for Board.cpp
        	void cleanup();

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

        	// Writes the board out to a stream
        	friend std::ostream& operator<< (std::ostream& os, const Game& game);

        	// Reads the board in from a stream
        	friend std::istream& operator>> (std::istream& is, Game& game);
	};
}
#endif // GAME_H
