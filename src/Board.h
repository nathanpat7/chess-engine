#pragma once
#include <iostream>
#include <sstream>
#include <climits>
#include "Piece.h"
#include "MoveList.h"
#include "PieceSquareTables.h"


class Board {
public:
	unsigned char board[128]; // 0x88 board representation
	unsigned char colorToMove;
	unsigned char enPassant;
	unsigned char whiteCastle; // 0: neither, 1: kingside, 2: queenside, 3: both
	unsigned char blackCastle;
	unsigned char halfMoves;
	unsigned short fullMoves;
	unsigned char depth;
	unsigned char kingPosition[2];
	unsigned char pieceLocations[2][16];
	bool simple_search;

	// Default constructor (clears board)
	Board();

	// Checks if a square is valid
	bool isSquareValid(unsigned char index);

	// Converts board index to algebraic notation i.e. "37 to f3"
	const char* indexToString(unsigned char index);

	// Converts algebraic notation to board index i.e. "f3 to 37"
	unsigned char stringToIndex(const char* squareString);

	// Generates pseudo-legal moves
	MoveList GenerateMoves();

	// Update board with move; returns true if legal
	bool makeMove(Move* move);

	// Returns true if squarePos is under attack by a piece of color: color
	bool isInCheck(unsigned char squarePos, unsigned char color);

	// Performance test; returns number of positions reached in given depth
	int perft(int depth);

	// Minimax-style algorithm with pruning; returns best move found
	double alphaBeta(Move &bestMove, int depth, double alpha, double beta, bool maximizingPlayer);

	// Returns piece value of a given piece
	int pieceValue(unsigned char piece);
	
	// Evaluate the current position
	double evaluatePosition();

	// Loads a position from a FEN string
	void loadPosition(std::string fen);

	// Prints a simple ASCII board interface
	void printBoard();
};
