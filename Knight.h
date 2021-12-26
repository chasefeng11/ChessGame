#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

namespace Chess {
	class Knight : public Piece {

	public:
		bool legal_move_shape(const Position& start, const Position& end) const override;

		char to_ascii() const override { return is_white() ? 'N' : 'n';	}
    
		std::string to_unicode() const override { return is_white() ? "\u2658" : "\u265E"; }

		int point_value() const override { return 3; }

	private:
		Knight(bool is_white) : Piece(is_white) {}

		friend Piece* create_piece(const char& piece_designator);
	};
}

#endif // KNIGHT_H
