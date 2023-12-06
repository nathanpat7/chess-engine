#pragma once
#include "Move.h"


// MoveList pool
class MoveList {
public:
	Move movesPool[218];
	unsigned char size;

	MoveList();
	void push_back(unsigned char startPos, unsigned char endPos, unsigned char moveType);
	Move* pop_front();
};