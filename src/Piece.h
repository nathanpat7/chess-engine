#pragma once


struct Piece {
	static const unsigned char BLACK = 8;
	static const unsigned char WHITE = 16;

	static const unsigned char NONE = 0;
	static const unsigned char PAWN = 1;
	static const unsigned char KNIGHT = 2;
	static const unsigned char BISHOP = 3;
	static const unsigned char ROOK = 4;
	static const unsigned char QUEEN = 5;
	static const unsigned char KING = 6;
};