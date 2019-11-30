#include "INCLUDE.h"

using std::string;

#define ENGINE


#ifdef ENGINE
int main() {
	char temp[100];

	scanf("%s", temp);
	if (strcmp(temp, "name?") == 0) {
		printf("%s\n", NAME);
		fflush(stdout);
	}

	int first = 0;
	scanf("%s", temp);
	if (strcmp(temp, "redfirst") == 0) {
		first = 1;
	}
	else if (strcmp(temp, "bluefirst") == 0) {
		first = 2;
	}

	int my_color = 0;
	scanf("%s", temp);
	if (strcmp(temp, "red") == 0) {
		my_color = 1;
	}
	else if (strcmp(temp, "blue") == 0) {
		my_color = 2;
	}

	string my_layout;
	/*if (my_color == 1) {
		my_layout = "162534";
	}
	else {
		my_layout = "164532";
	}*/
	my_layout = Utils::random_layout();

	printf("%s\n", my_layout.c_str());
	fflush(stdout);

	scanf("%s", temp);
	
	char opponent_layout[20];
	scanf("%s", opponent_layout);

	Board board;
	
	/*BoardUtil::set_layout(board, 1, LAYOUT);
	BoardUtil::set_layout(board, 2, opponent_layout);*/

	if (my_color == 2) {
		BoardUtil::set_layout(board, 1, opponent_layout);
		BoardUtil::set_layout(board, 2, my_layout.c_str());
	}
	else if (my_color == 1) {
		BoardUtil::set_layout(board, 1, my_layout.c_str());
		BoardUtil::set_layout(board, 2, opponent_layout);
	}
	//BoardUtil::print_board(board);

	//MCTSSolver::Info info;
	ABSolver::Info info;
	int turn = my_color;
	while (1) {
		if (turn == 1) {
			int dice;
			scanf("%d", &dice);

			int best_move;

			if (my_color == 1) {
				//best_move = MCTSSolver::solve(board, dice, &info);
				best_move = ABSolver::solve(board, dice, &info);
			}
			else {
				//best_move = MCTSSolver::solve(board.flip(), dice, &info);
				best_move = ABSolver::solve(board.flip(), dice, &info);
			}
			
			int x = best_move / 100;
			int y = best_move / 10 % 10;
			int dir = best_move % 10;
			int nx = x + DIR[dir][0];
			int ny = y + DIR[dir][1];

			if (my_color == 2) {
				BoardUtil::flip(x, y);
				BoardUtil::flip(nx, ny);
				dir = (dir + 4) % 8;
			}

			board.move(x, y, dir);

			printf("move %d%d %d%d %d\n", x, y, nx, ny, dice);
			printf("%.6lf\n", info.rate);
			//printf("%s\n", info.extra.c_str());
			fflush(stdout);
		}
		else if (turn == 2) {
			scanf("%s", temp);
			if (strcmp(temp, "move") == 0) {
				int coor, ncoor, dice;
				scanf("%d %d %d", &coor, &ncoor, &dice);
				int x = coor / 10;
				int y = coor % 10;
				int nx = ncoor / 10;
				int ny = ncoor % 10;

				for (int i = 0; i < 8; i++) {
					if (x + DIR[i][0] == nx && y + DIR[i][1] == ny) {
						board.move(x, y, i);
						break;
					}
				}
			}
			else if (strcmp(temp, "stop") == 0) {
				break;
			}
			else if (strcmp(temp, "regret") == 0) {
				// regret;
			}
			
		}

		//BoardUtil::print_board(board);
		turn = 3 - turn;
	}
}

#else

int main() {
	Board board = Board::load_from("D:/einstein/prob2.txt");

	MCTSSolver::Info info;
	//ABSolver::Info info;

	int turn = 1;

	while (1) {
		if (turn == 1) {
			int dice;

			scanf("%d", &dice);

			int move = MCTSSolver::solve(board, dice, &info);
			//int move = ABSolver::solve(board, dice, &info);
			printf("%d\n", move);
			printf("%lf\n", info.rate);
			//printf("%s\n", info.extra.c_str());

			board.move(move / 100, move / 10 % 10, move % 10);

			BoardUtil::print_board(board);
		}
		else if (turn == 2) {
			int move;

			scanf("%d", &move);

			board.move(move / 100, move / 10 % 10, move % 10);

			BoardUtil::print_board(board);
		}

		turn = 3 - turn;
	}
}

#endif