#include "INCLUDE.h"

int ABSolver::solve(const Board & board, int dice, Info * info) {
	int live_piece[7] = { 0 };
	int live_piece_pos[7][2] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = board.get(i, j);
			if (val > 0) {
				live_piece[val] = 1;
				live_piece_pos[val][0] = i;
				live_piece_pos[val][1] = j;
			}
		}
	}

	int i;
	for (i = dice; i >= 1 && live_piece[i] == 0; i--);
	if (i >= 1) {
		live_piece[i] = 2;
	}
	for (i = dice; i <= 6 && live_piece[i] == 0; i++);
	if (i <= 6) {
		live_piece[i] = 2;
	}

	double max_val = -INF;
	int best_move = 0;

	for (int i = 1; i <= 6; i++) {
		if (live_piece[i] != 2)
			continue;

		for (int dir = 4; dir <= 6; dir++) {
			int nx = live_piece_pos[i][0] + DIR[dir][0];
			int ny = live_piece_pos[i][1] + DIR[dir][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				Board b = board;
				b.move(live_piece_pos[i][0], live_piece_pos[i][1], dir);

				double val = 1 - naive_search(b, 4, 2);
				if (val > max_val) {
					max_val = val;
					best_move = live_piece_pos[i][0] * 100 + live_piece_pos[i][1] * 10 + dir;
				}
			}
		}
	}

	if (info) {
		info->rate = max_val;
	}

	return best_move;
}

// time for <player> to play
double ABSolver::alphabeta(
	Board & board,
	double alpha,
	double beta,
	int dep,
	int player
) {
	int gameover = BoardUtil::winner(board);
	if (gameover == 1) {
		return player == 1 ? 1 : 0;
	}
	else if (gameover == 2) {
		return player == 2 ? 1 : 0;
	}

	int weight[7] = { 0 };

	int live_piece[7] = { 0 };
	int live_piece_pos[7][2] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = board.get(i, j);
			if (val > 0 && player == 1) {
				live_piece[val] = 1;
				live_piece_pos[val][0] = i;
				live_piece_pos[val][1] = j;
			}
			else if (val < 0 && player == 2) {
				live_piece[-val] = 1;
				live_piece_pos[-val][0] = i;
				live_piece_pos[-val][1] = j;
			}
		}
	}
	for (int dice = 1; dice <= 6; dice++) {
		if (live_piece[dice] == 1) {
			weight[dice]++;
			continue;
		}
		int it = 0;
		for (it = dice - 1; it >= 1 && live_piece[it] == 0; it--);
		if (it >= 1) {
			weight[it]++;
		}
		for (it = dice + 1; it <= 6 && live_piece[it] == 0; it++);
		if (it <= 6) {
			weight[it]++;
		}
	}


	// generate valid move
	for (int i = 1; i <= 6; i++) {
		if (live_piece[i] == 0)
			continue;

		int l = player == 1 ? 4 : 0;
		int r = player == 1 ? 6 : 2;

		for (int dir = l; dir <= r; dir++) {
			int nx = live_piece_pos[i][0] + DIR[dir][0];
			int ny = live_piece_pos[i][1] + DIR[dir][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				Board b = board;
				b.move(live_piece_pos[i][0], live_piece_pos[i][1], dir);


				double val = -alphabeta(b, -beta, -alpha, dep - 1, 3 - player);
				//val *= weight[i] / 6.0;

				if (val >= beta) {
					return beta;
				}
				if (val > alpha) {
					alpha = val;
				}
			}
		}
	}

	return alpha;
}

// time for <player> to play
double ABSolver::naive_search(const Board & board, int dep, int player) {
	int gameover = BoardUtil::winner(board);
	if (gameover == 1) {
		return player == 1 ? 1 : 0;
	}
	else if (gameover == 2) {
		return player == 2 ? 1 : 0;
	}

	if (dep == 0) {
		int amount = 10;
		int win = Simulator::simulate(board, player, amount);
		double rate = 1.0 * win / amount;

		return player == 1 ? rate : 1 - rate;
	}

	int live_piece[7] = { 0 };
	int live_piece_pos[7][2] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = board.get(i, j);
			if (val > 0 && player == 1) {
				live_piece[val] = 1;
				live_piece_pos[val][0] = i;
				live_piece_pos[val][1] = j;
			}
			else if (val < 0 && player == 2) {
				live_piece[-val] = 1;
				live_piece_pos[-val][0] = i;
				live_piece_pos[-val][1] = j;
			}
		}
	}

	double rate[7] = { 0 };

	for (int i = 1; i <= 6; i++) {
		if (live_piece[i] == 0)
			continue;

		int l = player == 1 ? 4 : 0;
		int r = player == 1 ? 6 : 2;

		for (int dir = l; dir <= r; dir++) {
			int nx = live_piece_pos[i][0] + DIR[dir][0];
			int ny = live_piece_pos[i][1] + DIR[dir][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				Board b = board;
				b.move(live_piece_pos[i][0], live_piece_pos[i][1], dir);

				double val = 1 - naive_search(b, dep - 1, 3 - player);
				if (val > rate[i]) {
					rate[i] = val;
				}
			}
		}
	}

	double ans = 0;

	for (int dice = 1; dice <= 6; dice++) {
		if (live_piece[dice] == 1) {
			ans += rate[dice] / 6.0;
			continue;
		}
		int it = 0;
		double temp = 0;
		for (it = dice - 1; it >= 1 && live_piece[it] == 0; it--);
		if (it >= 1) {
			temp = rate[it];
		}
		for (it = dice + 1; it <= 6 && live_piece[it] == 0; it++);
		if (it <= 6) {
			temp = std::max(temp, rate[it]);
		}

		ans += temp / 6.0;
	}

	return ans;
}