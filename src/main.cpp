#include "Board.h"


int main() {
	std::string input;
	Board game;
	Move move;
	printf("Commands:\n\tload startpos\n\tload fen <string>\n\tprint board\n\tmove <from> <to> <type>\n\tsearch <depth>\n\tperft <depth>\n\teval\n\thelp\n\texit\n\n");
	printf("Move types:\n\t0: normal\n\t1: pawn forward 2\n\t2: en passant\n\t3: castling\n\t4: promotion:queen\n\t5: promotion:knight\n\t6: promotion:bishop\n\t7: promotion:rook\n\n");

	while (true) {
		printf(">>");
		std::getline(std::cin, input);
		std::istringstream iss(input);
		std::string token;

		if (!std::getline(iss, token, ' ')) {
			exit(-1);
		}

		if (token == "exit") {
			break;
		}
		if (token == "help") {
			printf("Commands:\n\tload startpos\n\tload fen <string>\n\tprint board\n\tmove <from> <to> <type>\n\tsearch <depth>\n\tperft <depth>\n\teval\n\thelp\n\texit\n\n");
			printf("Move types:\n\t0: normal, 1: pawn forward 2, 2: en passant, 3: castling, 4: promotion:queen, 5: promotion:knight, 6: promotion:bishop, 7: promotion:rook\n\n");
			continue;
		}
		if (token == "load") {
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			if (token == "startpos") {
				game.loadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
			}
			if (token == "fen") {
				if (!std::getline(iss, token)) {
					exit(-1);
				}
				game.loadPosition(token);
			}
			continue;
		}
		if (token == "print") {
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			if (token == "board") {
				game.printBoard();
			}
			continue;
		}
		if (token == "move") {
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			unsigned char from = game.stringToIndex(token.c_str());
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			unsigned char to = game.stringToIndex(token.c_str());
			if (!std::getline(iss, token, ' ')) {
				token = "0";
			}
			unsigned char type = token[0] - '0';
			move = Move(from, to, type);
			MoveList moves = game.GenerateMoves();
			bool legal_move = false;
			Board currentPosition = game;
			Move* currentMove;
			while ((currentMove = moves.pop_front()) != nullptr) {
				if (currentMove->from == move.from && currentMove->to == move.to && currentMove->type == move.type) {
					if (game.makeMove(&move)) {
						legal_move = true;
					}
					else {
						game = currentPosition;
					}
				}
			}
			if (!legal_move) {
				printf("Illegal move\n\n");
			}
			continue;
		}
		if (token == "perft") {
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			unsigned char depth = token[0] - '0';
			game.depth = depth;
			clock_t start_time = clock();
			printf("Depth: %d\nPositions: %d\n", game.depth, game.perft(game.depth));
			clock_t end_time = clock();
			double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
			printf("Time: %.3f\n\n", elapsed_time);
			continue;
		}
		if (token == "search") {
			if (!std::getline(iss, token, ' ')) {
				exit(-1);
			}
			unsigned char depth = token[0] - '0';
			game.depth = depth;
			Move bestMove;

			clock_t start_time = clock();
			double eval = game.alphaBeta(bestMove, game.depth, -INT_MAX, INT_MAX, game.colorToMove == Piece::WHITE);
			if (int(eval) == 1000 || int(eval) == -1000) {
				printf("Checkmate found!\n");
			}
			else {
				printf("No checkmate found\n");
			}

			printf("Evaluation at depth %d: %.2f\n", game.depth, eval);
			printf("Best move: %s -> ", game.indexToString(bestMove.from));
			printf("%s", game.indexToString(bestMove.to));
			if (bestMove.type > 3) {
				if (bestMove.type == 4) {
					printf("=Q");
				}
				if (bestMove.type == 5) {
					printf("=N");
				}
				if (bestMove.type == 6) {
					printf("=B");
				}
				if (bestMove.type == 7) {
					printf("=R");
				}
			}

			clock_t end_time = clock();
			double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
			printf("\nTime: %.3f\n\n", elapsed_time);
			continue;
		}
		if (token == "eval") {
			printf("Current position evaluation: %.2f\n\n", game.evaluatePosition());
			continue;
		}
		else {
			printf("Unknown command\n\n");
		}
	}
	return 0;
}