#include <cassert>
#include <cstring>
#include "Game.h"
#include "Helper.h"

namespace Chess
{
    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
    Game::Game() : is_white_turn(true) {
        // Add the pawns
        for (int i = 0; i < 8; i++) {
            board.add_piece(Position('A' + i, '1' + 1), 'P');
            board.add_piece(Position('A' + i, '1' + 6), 'p');
        }

        // Add the rooks
        board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
        board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
        board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
        board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

        // Add the knights
        board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
        board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
        board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
        board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

        // Add the bishops
        board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
        board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
        board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
        board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

        // Add the kings and queens
        board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
        board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
        board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
        board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
    }

    // Copy constructor
    Game::Game(const Game &game){
        *this = game;
    }

    // Assignment operator
    Game& Game::operator=(const Game &game){
        cleanup();
        this->board = Board(game.board);
        this->is_white_turn = game.is_white_turn;
        return *this;
    }

    // Promotion function
    bool Game::check_promotion(const Position& start, const Position& end) const {
        const Piece* start_piece = board(start);

        // If piece is not a pawn, promotion can not be done
        if(start_piece->to_ascii() != 'p' && start_piece->to_ascii() != 'P'){
            return false;
        }

        // Checks if piece is at opposite end of board
        if(start_piece->is_white() && (end.second == '8')){
            return true;
        }
        // Condition for black pawn
        else if(!start_piece->is_white() && (end.second == '1')){
            return true;
        }

        return false;

    }

    // Function to move pieces on the chess board
    void Game::make_move(const Position& start, const Position& end) {

        // Throw exceptions if player tries to make an illegal move
        if(start.first > 'H' || start.first < 'A' || start.second < '1' || start.second > '8')
            throw Exception("start position is not on board");

        if(end.first > 'H' || end.first < 'A' || end.second < '1' || end.second > '8')
            throw Exception("end position is not on board");

        const Piece* start_piece = board(start);

        if(start_piece == nullptr)
            throw Exception("no piece at start position");

        if(start_piece->is_white() != turn_white())
            throw Exception("piece color and turn do not match");

        const Piece* end_piece = board(end);

        // Checks if something exists where player is trying to go
        // Capture Move Case
        if(end_piece != nullptr){

            // But that place is occupied by my piece
            if(end_piece->is_white() == turn_white())
                throw Exception("cannot capture own piece");

            // But the path is illegal for the selected piece
            if(!start_piece->legal_capture_shape(start, end))
                throw Exception("illegal capture shape");
        }

        // Non Capture Move Case
        else{
            if(!start_piece->legal_move_shape(start, end))
                throw Exception("illegal move shape");
        }

        // As per Piazza, only check if is_path_clear() if it's NOT diagonal, vertical & horizontal
        // ^Accommodates for bishop and mystery piece
        if(is_path_linear(start, end) && !is_path_clear(start, end)){
            throw Exception("path is not clear");
        }

        // If it is a capture move, removes the piece at the end
        if(end_piece != nullptr){
            // Removes captured piece
            board.remove_piece(end);
        }

        // Do not allow a move if it will result in check
        if (would_check(start, end))
            throw Exception("this move causes a check");
        
        char piece_designator;

        //Promotes piece if necesarry
        if(check_promotion(start, end)){
            piece_designator = turn_white() ? 'Q' : 'q';
        }else{
            piece_designator = start_piece->to_ascii();
        }

        // First, add the piece to the requested location
        board.add_piece(end, piece_designator);

        // Then, removes piece at original location
        board.remove_piece(start);

        // Finally, changes turns
        is_white_turn = !is_white_turn;

	}

    // Checks if the piece's path is linear
    // Helpful when dealing with bishop and mystery piece
	bool Game::is_path_linear(const Position& start, const Position& end) const {
        // Vertical & horizontal
        if(start.first == end.first || start.second == end.second){
            return true;
        }

        // Checks to see if piece moved in a diagonal
        if(ABS(start.first - end.first) == ABS(start.second - end.second)){
            return true;
        }

        return false;
    }

    // Checks if the entered path is free of obstacles
    bool Game::is_path_clear(const Position &start, const Position &end) const {
	    // This function assumes that all moves are legal - only checks
	    // if the path is clear to the requested location.

	    char start_col = start.first;
	    char start_row = start.second;

        char end_col = end.first;
        char end_row = end.second;

        if(start_row == end_row || start_col == end_col){
            // If going in a straight line

            if(start_col == end_col){
                // Going up and down (vertical)

                while(start_row != end_row){
                    // Check if the current position has any pieces
                    const Piece* piece = board(Position(start_col, start_row));

                    // If it does, return false
                    // Don't check on the first iteration of the loop, as the
                    // piece that the user is trying to move occupies that space.
                    if(start_row != start.second && piece != nullptr)
                        return false;

                    // If it doesn't, increase or decrease the current row
                    // and keep checking until it is the same
                    // NOTE: The final position isn't checked as the user might
                    // capture the piece at that position.
                    if(start_row < end_row)
                        start_row++;
                    else
                        start_row--;
                }

            }else{
                // Going left and right (horizontal)
                // Same implementation as above, just horizontally
                while(start_col != end_col){
                    const Piece* piece = board(Position(start_col, start_row));
                    if(start_col != start.first && piece != nullptr)
                        return false;

                    if(start_col < end_col)
                        start_col++;
                    else
                        start_col--;
                }
            }
        }else{
            // If going diagonally
            while(start_col != end_col){
                const Piece* piece = board(Position(start_col, start_row));

                // If it is the first move, skip
                if(start_col != start.first && piece != nullptr)
                    return false;

                if(start_col < end_col)
                    start_col++;
                else
                    start_col--;

                if(start_row < end_row)
                    start_row++;
                else
                    start_row--;
            }
        }

        return true;
    }

    // Function to see if move would result in a check if made
    bool Game::would_check(const Position &start, const Position &end) const {
        
        // Creates new Game to test out move
        Game new_game = Game(*this);

        // Since this would run at the very end, performing all necessary checks,
        // it would be safe to assume moving would be legal.

        // Replicating setup on new board
        const Piece* start_piece = new_game.board(start);
        new_game.board.add_piece(end, start_piece->to_ascii());
        new_game.board.remove_piece(start);

        // Returns whether move would create check scenario
        return new_game.in_check(new_game.turn_white());
    }

    // Function to determine if a player is in check
    bool Game::in_check(const bool& white) const {
        // Find location of correct king
        char piece_designator = white ? 'K' : 'k';
        const Position king_pos = board.find_by_piece(piece_designator);

        // Loops through each opposing piece to see if any have a check
        for (Board::const_iterator it = board.cbegin(); it != board.cend(); ++it){ //loop through map keys
            const Piece * piece = board(it.current_pos());

            // Creates a new hypothetical game to check and see piece could capture king
            Game new_game = Game(*this);

            if (piece != nullptr && piece->is_white() != white){
                if(piece->legal_capture_shape(it.current_pos(), king_pos)){
                    if(new_game.is_path_clear(it.current_pos(), king_pos)){
                        return true;
                    }
                }
            } 
        }
        return false;
    }

    // Checks if a given piece has any possible moves
    // Loops through each position in board, and sees if our piece can move there
    bool Game::is_possible_move(const Position& pos) const {
        for (Board::const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            Game new_game = Game(*this);
            try {
                new_game.make_move(pos, cit.current_pos());
            } catch(Exception& exception){
                continue;
            }
            return true;
        }
        return false;
    }

    // If a) there is no check, or b) a move can be made to prevent a check, returns false
    // To check b), loops through each piece to and calls prevent_check()
    bool Game::in_mate(const bool& white) const {
        if(!in_check(white)){
            return false;
        }

        // Check if moving other pieces can escape a check
        for (Board::const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            const Piece * piece = board(cit.current_pos());
            if (piece != nullptr && piece->is_white() == white){
                if (prevent_check(cit.current_pos())){
                    return false;
                }
            }
        }

        return true;
    }

    // Determine if another piece could be moved for king to escape check
    bool Game::prevent_check(const Position& pos) const {
        // Iterate through board
        for (Board::const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            Game new_game = Game(*this);
            try {
                new_game.make_move(pos, cit.current_pos());
            } catch(Exception& exception){
                continue;
            }
            return true;
        }
        return false;
    }

    // Checks if every one of a color's pieces have no possible moves
    // Calls is_possible_move for each piece
    bool Game::in_stalemate(const bool& white) const {
        for (Board::const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            const Piece * piece = board(cit.current_pos());

            if (piece != nullptr && piece->is_white() == white){
                if (is_possible_move(cit.current_pos())){
                    return false;
                }
            }
        }

        return true;
    }

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {
        int point_val = 0;
        for (Board::const_iterator cit = board.cbegin(); cit != board.cend(); ++cit){
            const Piece * piece = board(cit.current_pos());

            if (piece != nullptr && piece->is_white() == white){
                point_val += piece->point_value();
            }
        }
        return point_val;
    }

    // Overload >> operator to help load a game from a file
    std::istream& operator>> (std::istream& is, Game& game) {

        game.board.remove_all();

        // add_piece() will throw an exception if any piece other than the designated ones
        for(int row = '8'; row >= '1'; row--){
            for(int col = 'A'; col <= ('H' + 1); col++){
                char piece_designator = is.get();
                if(piece_designator != '-' && piece_designator != '\n')
                    game.board.add_piece(Position(col, row), piece_designator);
            }
        }

        char turn_designator = is.get();

        if(turn_designator != 'w' && turn_designator != 'b')
            throw Exception();

        game.is_white_turn = turn_designator == 'w';

        return is;
    }

    // Destructor
    Game::~Game(){ }

    void Game::cleanup(){
        board.cleanup();
    }

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
    std::ostream& operator<< (std::ostream& os, const Game& game) {
        // Write the board out and then either the character 'w' or the
        // character 'b',
        // depending on whose turn it is
        return os << game.board << (game.turn_white() ? 'w' : 'b');
    }
}
