#include "Board.h"


// Default constructor (clears board)
Board::Board() {
	std::memset(this, 0, sizeof(Board));
	enPassant = -2;
	std::memset(pieceLocations, -2, sizeof(pieceLocations));
}

// Checks if a square is valid
bool Board::isSquareValid(unsigned char index) {
	return (index & 0x88) == 0;
}

// Converts board index to algebraic notation i.e. "37 to f3"
const char* Board::indexToString(unsigned char index) {
	static char squareString[3];
	if (isSquareValid(index)) {
		squareString[0] = 'a' + index % 16;
		squareString[1] = '1' + index / 16;
		squareString[2] = '\0';
	}
	else {
		squareString[0] = 'x';
		squareString[1] = 'x';
		squareString[2] = '\0';
	}
	return squareString;
}

// Converts algebraic notation to board index i.e. "f3 to 37"
unsigned char Board::stringToIndex(const char* squareString) {
	unsigned char index = squareString[0] - 'a' + 16 * (squareString[1] - '1');
	if (isSquareValid(index)) {
		return index;
	}
	else {
		return -1;
	}
}

// Generates pseudo-legal moves
MoveList Board::GenerateMoves() {
	MoveList moves;

	bool colorIndex = colorToMove == Piece::BLACK;
	for (int i = 0; i < 128; i++) {
		if (!isSquareValid(i)) {
			continue;
		}
		unsigned char square = board[i];
		if (square == Piece::NONE || (square & 0x18) != colorToMove) {
			continue;
		}

		unsigned char startPos = i, endPos, endSquare;

		// Direction offset indices
		char direction[] = {16, -16, 1, -1, 17, -17, 15, -15};
		char knightDirection[] = {31, 33, 18, -14, -31, -33, -18, 14}; // clockwise starting top left
		char pawnForward = colorToMove * 4 - 48;
		char pawnDirection[] = {pawnForward, char(2 * pawnForward), char(pawnForward - 1), char(pawnForward + 1)};

		// Determine piece type and generate moves
		switch (square & 0x07) {
			case Piece::PAWN:
				// Forward one
				endPos = startPos + pawnDirection[0];
				endSquare = board[endPos];
				if (endSquare == Piece::NONE) {
					if (int(endPos / 16) == 0 || int(endPos / 16) == 7) {
						moves.push_back(startPos, endPos, 4);
						moves.push_back(startPos, endPos, 5);
						moves.push_back(startPos, endPos, 6);
						moves.push_back(startPos, endPos, 7);
					}
					else {
						moves.push_back(startPos, endPos, 0);
					}

					// Forward two
					if (int(startPos / 16) == 1 || int(startPos / 16) == 6) {
						endPos = startPos + pawnDirection[1];
						if (isSquareValid(endPos)) {
							endSquare = board[endPos];
							if (endSquare == Piece::NONE) {
								moves.push_back(startPos, endPos, 1);
							}
						}
					}
				}

				for (int dIndex = 2; dIndex < 4; dIndex++) {
					endPos = startPos + pawnDirection[dIndex];
					endSquare = board[endPos];
					if (endSquare != Piece::NONE && (endSquare & 0x18) != colorToMove) {
						if (int(endPos / 16) == 0 || int(endPos / 16) == 7) {
							moves.push_back(startPos, endPos, 4);
							moves.push_back(startPos, endPos, 5);
							moves.push_back(startPos, endPos, 6);
							moves.push_back(startPos, endPos, 7);
						}
						else {
							moves.push_back(startPos, endPos, 0);
						}
					}
					if (endPos == enPassant) {
						moves.push_back(startPos, endPos, 2);
					}
				}
				break;

			case Piece::KNIGHT:
				for (int dIndex = 0; dIndex < 8; dIndex++) {
					endPos = startPos + knightDirection[dIndex];
					if (!isSquareValid(endPos)) {
						continue;
					}
					endSquare = board[endPos];
					if (endSquare == Piece::NONE || (endSquare & 0x18) != colorToMove) {
						moves.push_back(startPos, endPos, 0);
					}
				}
				break;

			case Piece::BISHOP:
				for (int dIndex = 4; dIndex < 8; dIndex++) {
					endPos = startPos;
					while (true) {
						endPos += direction[dIndex];
						if (!isSquareValid(endPos)) {
							break;
						}
						endSquare = board[endPos];
						if (endSquare == Piece::NONE) {
							moves.push_back(startPos, endPos, 0);
							continue;
						}
						if ((endSquare & 0x18) != colorToMove) {
							moves.push_back(startPos, endPos, 0);
						}
						break;
					}
				}
				break;

			case Piece::ROOK:
				for (int dIndex = 0; dIndex < 4; dIndex++) {
					endPos = startPos;
					while (true) {
						endPos += direction[dIndex];
						if (!isSquareValid(endPos)) {
							break;
						}
						endSquare = board[endPos];
						if (endSquare == Piece::NONE) {
							moves.push_back(startPos, endPos, 0);
							continue;
						}
						if ((endSquare & 0x18) != colorToMove) {
							moves.push_back(startPos, endPos, 0);
						}
						break;
					}
				}
				break;

			case Piece::QUEEN:
				for (int dIndex = 0; dIndex < 8; dIndex++) {
					endPos = startPos;
					while (true) {
						endPos += direction[dIndex];
						if (!isSquareValid(endPos)) {
							break;
						}
						endSquare = board[endPos];
						if (endSquare == Piece::NONE) {
							moves.push_back(startPos, endPos, 0);
							continue;
						}
						if ((endSquare & 0x18) != colorToMove) {
							moves.push_back(startPos, endPos, 0);
						}
						break;
					}
				}
				break;

			case Piece::KING:
				for (int dIndex = 0; dIndex < 8; dIndex++) {
					endPos = startPos + direction[dIndex];
					if (!isSquareValid(endPos)) {
						continue;
					}
					endSquare = board[endPos];
					if (endSquare == Piece::NONE || (endSquare & 0x18) != colorToMove) {
						moves.push_back(startPos, endPos, 0);
					}
				}

				// Castling
				if (colorToMove == Piece::WHITE) {
					if (whiteCastle == 1 || whiteCastle == 3) {
						if (board[5] == Piece::NONE && board[6] == Piece::NONE) {
							if (!isInCheck(4, 24 - colorToMove) && !isInCheck(5, 24 - colorToMove)) {
								moves.push_back(startPos, 6, 3);
							}
						}
					}
					if (whiteCastle == 2 || whiteCastle == 3) {
						if (board[1] == Piece::NONE && board[2] == Piece::NONE && board[3] == Piece::NONE) {
							if (!isInCheck(4, 24 - colorToMove) && !isInCheck(3, 24 - colorToMove)) {
								moves.push_back(startPos, 2, 3);
							}
						}
					}
				}
				else {
					if (blackCastle == 1 || blackCastle == 3) {
						if (board[117] == Piece::NONE && board[118] == Piece::NONE) {
							if (!isInCheck(116, 24 - colorToMove) && !isInCheck(117, 24 - colorToMove)) {
								moves.push_back(startPos, 118, 3);
							}
						}
					}
					if (blackCastle == 2 || blackCastle == 3) {
						if (board[113] == Piece::NONE && board[114] == Piece::NONE && board[115] == Piece::NONE) {
							if (!isInCheck(116, 24 - colorToMove) && !isInCheck(115, 24 - colorToMove)) {
								moves.push_back(startPos, 114, 3);
							}
						}
					}
				}
				break;
		}
	}
	return moves;
}

// Update board with move; returns true if legal
bool Board::makeMove(Move* move) {
	for (int i = 0; i < 2; i++) {
		if (move->from == kingPosition[i]) {
			kingPosition[i] = move->to;
		}
	}

	bool colorIndex = colorToMove == Piece::BLACK;
	bool capture = false;
	if (board[move->to] != Piece::NONE) {
		capture = true;
	}

	enPassant = -2;

	// castling rights
	if (move->from == 4) {
		whiteCastle = 0;
	}
	if (move->from == 116) {
		blackCastle = 0;
	}
	if (move->from == 7 || move->to == 7) {
		if (whiteCastle % 2 == 1) {
			whiteCastle--;
		}
	}
	if (move->from == 0 || move->to == 0) {
		if (whiteCastle > 1) {
			whiteCastle -= 2;
		}
	}
	if (move->from == 119 || move->to == 119) {
		if (blackCastle % 2 == 1) {
			blackCastle--;
		}
	}
	if (move->from == 112 || move->to == 112) {
		if (blackCastle > 1) {
			blackCastle -= 2;
		}
	}


	// Move types
	if (move->type == 0) {
		board[move->to] = board[move->from];
		board[move->from] = Piece::NONE;
	}

	if (move->type == 1) {
		board[move->to] = board[move->from];
		board[move->from] = Piece::NONE;
		enPassant = (move->to + move->from) / 2;
	}

	if (move->type == 2) {
		board[move->to] = board[move->from];
		board[move->from] = Piece::NONE;
		board[move->to + colorToMove * -4 + 48] = Piece::NONE;
	}

	// castling -- CLEAN UP
	if (move->type == 3) {
		board[move->to] = board[move->from];
		board[move->from] = Piece::NONE;
		board[int((move->to + move->from) / 2)] = Piece::ROOK | colorToMove;
		if (move->to == 6) {
			board[7] = Piece::NONE;
			whiteCastle = 0;
		}
		if (move->to == 2) {
			board[0] = Piece::NONE;
			whiteCastle = 0;
		}
		if (move->to == 118) {
			board[119] = Piece::NONE;
			blackCastle = 0;
		}
		if (move->to == 114) {
			board[112] = Piece::NONE;
			blackCastle = 0;
		}
	}

	if (move->type == 4) {
		board[move->to] = Piece::QUEEN | colorToMove;
		board[move->from] = Piece::NONE;
	}
	if (move->type == 5) {
		board[move->to] = Piece::KNIGHT | colorToMove;
		board[move->from] = Piece::NONE;
	}
	if (move->type == 6) {
		board[move->to] = Piece::BISHOP | colorToMove;
		board[move->from] = Piece::NONE;
	}
	if (move->type == 7) {
		board[move->to] = Piece::ROOK | colorToMove;
		board[move->from] = Piece::NONE;
	}

	// filter out illegal moves
	if (isInCheck(kingPosition[colorIndex], 24 - colorToMove)) {
		return 0;
	}

	if (colorToMove == Piece::BLACK) {
		fullMoves++;
	}

	// Toggle between White and Black
	colorToMove = 24 - colorToMove;

	return 1;
}

// Returns true if squarePos is under attack by a piece of color: color
bool Board::isInCheck(unsigned char squarePos, unsigned char color) {
	unsigned char endPos, endSquare;

	char direction[] = {16, -16, 1, -1, 17, -17, 15, -15};
	char knightDirection[] = {31, 33, 18, -14, -31, -33, -18, 14}; // clockwise starting top left
	char pawnForward = color * -4 + 48;
	char pawnDirection[] = {pawnForward, char(2 * pawnForward), char(pawnForward - 1), char(pawnForward + 1)};

	for (int dIndex = 0; dIndex < 4; dIndex++) {
		endPos = squarePos;
		while (true) {
			endPos += direction[dIndex];
			if (!isSquareValid(endPos)) {
				break;
			}
			endSquare = board[endPos];
			if (endSquare == Piece::NONE) {
				continue;
			}
			if (endSquare == (Piece::ROOK | color) || endSquare == (Piece::QUEEN | color)) {
				return 1;
			}
			break;
		}
	}

	for (int dIndex = 4; dIndex < 8; dIndex++) {
		endPos = squarePos;
		while (true) {
			endPos += direction[dIndex];
			if (!isSquareValid(endPos)) {
				break;
			}
			endSquare = board[endPos];
			if (endSquare == Piece::NONE) {
				continue;
			}
			if (endSquare == (Piece::BISHOP | color) || endSquare == (Piece::QUEEN | color)) {
				return 1;
			}
			break;
		}
	}

	for (int dIndex = 0; dIndex < 8; dIndex++) {
		endPos = squarePos + knightDirection[dIndex];
		if (!isSquareValid(endPos)) {
			continue;
		}
		endSquare = board[endPos];
		if (endSquare == (Piece::KNIGHT | color)) {
			return 1;
		}
	}

	for (int dIndex = 0; dIndex < 8; dIndex++) {
		endPos = squarePos + direction[dIndex];
		if (!isSquareValid(endPos)) {
			continue;
		}
		endSquare = board[endPos];
		if (endSquare == (Piece::KING | color)) {
			return 1;
		}
	}

	for (int dIndex = 2; dIndex < 4; dIndex++) {
		endPos = squarePos + pawnDirection[dIndex];
		if (!isSquareValid(endPos)) {
			continue;
		}
		endSquare = board[endPos];
		if (endSquare == (Piece::PAWN | color)) {
			return 1;
		}
	}

	return 0;
}

// Performance test; returns number of positions reached in given depth
int Board::perft(int depth) {
	if (depth == 0) {
		return 1;
	}

	Board currentPosition = *this;
	MoveList moves = GenerateMoves();
	int numPositions = 0;
	Move* currentMove;
	while ((currentMove = moves.pop_front()) != nullptr) {
		if (makeMove(currentMove)) {
			int positions = perft(depth - 1);
			if (depth == this->depth) {
				printf("%s -> ", indexToString(currentMove->from));
				printf("%s", indexToString(currentMove->to));
				if (currentMove->type > 3) {
					if (currentMove->type == 4) {
						printf("=Q");
					}
					if (currentMove->type == 5) {
						printf("=N");
					}
					if (currentMove->type == 6) {
						printf("=B");
					}
					if (currentMove->type == 7) {
						printf("=R");
					}
				}
				printf(": %d\n", positions);
			}
			numPositions += positions;
		}
		*this = currentPosition;
	}
	return numPositions;
}

// Minimax-style algorithm with pruning; returns best move found
double Board::alphaBeta(Move &bestMove, int depth, double alpha, double beta, bool maximizingPlayer) {
	if (depth == 0) {
		return evaluatePosition();
	}

	MoveList moves = GenerateMoves();

	double bestValue = maximizingPlayer ? -1001 : 1001;
	bool terminal = true;
	Board currentPosition = *this;

	Move* currentMove;
	while ((currentMove = moves.pop_front()) != nullptr) {
		double value;

		// make move
		if (!makeMove(currentMove)) {
			*this = currentPosition;
			continue;
		}
		terminal = false;

		// get value of move
		value = alphaBeta(bestMove, depth - 1, alpha, beta, !maximizingPlayer);

		// update best value and best move
		if ((maximizingPlayer && value > bestValue) || (!maximizingPlayer && value < bestValue)) {
			bestValue = value;
			if (depth == this->depth) {
				bestMove = Move(currentMove->from, currentMove->to, currentMove->type);
			}
		}

		// update alpha and beta
		if (maximizingPlayer) {
			alpha = std::max(alpha, value);
		}
		else {
			beta = std::min(beta, value);
		}

		// revert move
		*this = currentPosition;

		if (alpha >= beta) {
			break;
		}
	}

	// check if terminal (checkmate or stalemate)
	if (terminal) {
		unsigned char kingPos;
		for (int i = 0; i < 128; i++) {
			if (board[i] == (Piece::KING | colorToMove)) {
				kingPos = i;
				break;
			}
		}

		if (isInCheck(kingPos, 24 - colorToMove)) {
			if (colorToMove == Piece::WHITE) {
				return -1000;
			}
			return 1000;
		}
		return 0;
	}

	return bestValue;
}

// Returns piece value of a given piece
int Board::pieceValue(unsigned char piece) {
	if (piece == Piece::PAWN) {
		return 100;
	}
	else if (piece == Piece::KNIGHT) {
		return 320;
	}
	else if (piece == Piece::BISHOP) {
		return 330;
	}
	else if (piece == Piece::ROOK) {
		return 510;
	}
	else if (piece == Piece::QUEEN) {
		return 880;
	}
	return 0;
}

// Evaluate the current position
double Board::evaluatePosition() {
	if (simple_search) {
		return 0;
	}

	int evaluation = 0;
	int totalMaterial = 0;
	unsigned char rank;
	unsigned char file;

	for (int i = 0; i < 128; i++) {
		unsigned char piece = board[i];
		if (piece == Piece::NONE) {
			continue;
		}

		rank = i / 16;
		file = i % 16;
		if (piece > Piece::WHITE) {
			rank = 7 - rank;
		}
		
		int pieceEval = pieceValue(piece & 0x07);

		switch (piece & 0x07) {
		case Piece::PAWN:
			pieceEval += PieceSquareTables::pawnTable[rank * 8 + file];
			break;

		case Piece::KNIGHT:
			pieceEval += PieceSquareTables::knightTable[rank * 8 + file];
			break;

		case Piece::BISHOP:
			pieceEval += PieceSquareTables::bishopTable[rank * 8 + file];
			break;

		case Piece::ROOK:
			pieceEval += PieceSquareTables::rookTable[rank * 8 + file];
			break;

		case Piece::QUEEN:
			pieceEval += PieceSquareTables::queenTable[rank * 8 + file];
			break;
		}

		totalMaterial += pieceEval;

		if (piece > Piece::WHITE) {
			evaluation += pieceEval;
		}
		else {
			evaluation -= pieceEval;
		}
	}

	// King position value interpolation
	rank = 7 - kingPosition[0] / 16;
	file = kingPosition[0] % 16;
	signed char kingMiddle = PieceSquareTables::kingMiddleTable[rank * 8 + file];
	signed char kingEnd = PieceSquareTables::kingEndTable[rank * 8 + file];
	evaluation += kingMiddle + (kingEnd - kingMiddle) * (1 - totalMaterial / 8000.0);

	rank = kingPosition[1] / 16;
	file = kingPosition[1] % 16;
	kingMiddle = PieceSquareTables::kingMiddleTable[rank * 8 + file];
	kingEnd = PieceSquareTables::kingEndTable[rank * 8 + file];
	evaluation -= kingMiddle + (kingEnd - kingMiddle) * (1 - totalMaterial / 8000.0);

	return evaluation / 100.0;
}

// Loads a position from a FEN string
void Board::loadPosition(std::string fen) {
	// Reset board
	std::memset(this, 0, sizeof(Board));
	enPassant = -2;

	std::istringstream iss(fen);
	std::string token;

	// Set up pieces
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	unsigned char file = 0, rank = 7;
	unsigned char pieceIndex[2] = { 0, 0 };
	for (char c : token) {
		if (c == '/') {
			file = 0;
			rank--;
		}
		else if (std::isdigit(c)) {
			file += c - '0';
		}
		else {
			unsigned char color, piece;
			color = (std::isupper(c)) ? Piece::WHITE : Piece::BLACK;
			c = std::tolower(c);

			switch (c) {
				case 'p':
					piece = Piece::PAWN;
					break;
				case 'n':
					piece = Piece::KNIGHT;
					break;
				case 'b':
					piece = Piece::BISHOP;
					break;
				case 'r':
					piece = Piece::ROOK;
					break;
				case 'q':
					piece = Piece::QUEEN;
					break;
				case 'k':
					piece = Piece::KING;
					break;
				default:
					printf("loadGameState() error: Invalid FEN string\n");
					exit(-1);
			}

			// Update the board and that piece's bitboard
			bool colorIndex = color == Piece::BLACK;

			if (piece != Piece::NONE) {
				pieceLocations[colorIndex][pieceIndex[colorIndex]] = rank * 16 + file;
				pieceIndex[colorIndex]++;
			}

			if (piece == Piece::KING) {
				kingPosition[colorIndex] = rank * 16 + file;
			}
			piece |= color;
			board[rank * 16 + file] = piece;
			file++;
		}
	}

	// Side to move
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	if (token[0] == 'w') {
		colorToMove = Piece::WHITE;
	}
	if (token[0] == 'b') {
		colorToMove = Piece::BLACK;
	}

	// Castling rights
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	for (char c : token) {
		if (c == '-') {
			break;
		}
		switch (c) {
			case 'K':
				whiteCastle += 1;
				break;
			case 'Q':
				whiteCastle += 2;
				break;
			case 'k':
				blackCastle += 1;
				break;
			case 'q':
				blackCastle += 2;
				break;
			default:
				printf("loadGameState() error: Invalid FEN string\n");
				exit(-1);
		}
	}

	// En passant
	file = 0, rank = 0;
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	if (token != "-") {
		enPassant = stringToIndex(token.c_str());
	}

	// Half moves
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	halfMoves = std::stoi(token);

	// Full moves
	if (!std::getline(iss, token, ' ')) {
		printf("loadGameState() error: Invalid FEN string\n");
		exit(-1);
	}
	fullMoves = std::stoi(token);
}

// Prints a simple ASCII board interface
void Board::printBoard() {
	printf("\n\t    a   b   c   d   e   f   g   h");
	for (int rank = 7; rank >= 0; rank--) {
		printf("\n\t  +---+---+---+---+---+---+---+---+\n\t%d |", rank + 1);
		for (int file = 0; file < 8; file++) {
			char pieceChar;
			switch (board[rank * 16 + file] & 0x07) {
				case Piece::NONE:
					pieceChar = ((file + rank) % 2) ? ' ' : ' ';
					break;
				case Piece::PAWN:
					pieceChar = 'p';
					break;
				case Piece::KNIGHT:
					pieceChar = 'n';
					break;
				case Piece::BISHOP:
					pieceChar = 'b';
					break;
				case Piece::ROOK:
					pieceChar = 'r';
					break;
				case Piece::QUEEN:
					pieceChar = 'q';
					break;
				case Piece::KING:
					pieceChar = 'k';
					break;
				default:
					printf("\n\nprintBoard() error: Invalid piece in square\n");
					exit(-1);
			}

			if (board[rank * 16 + file] < 16) {
				printf(" %c |", pieceChar);
			}
			else {
				printf(" %c |", std::toupper(pieceChar));
			}
		}
		printf(" %d", rank + 1);
	}
	printf("\n\t  +---+---+---+---+---+---+---+---+\n");
	printf("\t    a   b   c   d   e   f   g   h\n");
}
