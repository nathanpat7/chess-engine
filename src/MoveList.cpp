#include "MoveList.h"


MoveList::MoveList() {
	std::memset(this, 0, sizeof(MoveList));
}

void MoveList::push_back(unsigned char startPos, unsigned char endPos, unsigned char moveType) {
	if (size > 217) {
		return;
	}
	Move newMove(startPos, endPos, moveType);
	movesPool[size] = newMove;
	size++;
}

Move* MoveList::pop_front() {
	if (size == 0) {
		return nullptr;
	}

	size--;
	return &movesPool[size];
}