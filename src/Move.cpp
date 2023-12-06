#include "Move.h"


Move::Move() {
	std::memset(this, 0, sizeof(Move));
}

Move::Move(unsigned char startPos, unsigned char endPos, unsigned char moveType) {
	from = startPos;
	to = endPos;
	type = moveType;
}