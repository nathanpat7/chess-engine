#pragma once
#include <cstring>


class Move {
public:
	unsigned char from;
	unsigned char to;
	unsigned char type; // 0 - normal, 1 - pawn forward 2, 2 - en passant, 3 - castling, 4 - promotion:queen, 5 - promotion:knight, 6 - promotion:bishop, 7 - promotion:rook

	Move();
	Move(unsigned char startPos, unsigned char endPos, unsigned char moveType);
};