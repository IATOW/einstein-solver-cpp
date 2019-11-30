#include "INCLUDE.h"

using std::vector;
using std::pair;
using std::make_pair;
using std::string;

MCTSSolver::Node * MCTSSolver::find_most_visited_node(Node * p) {
	int M = 0;

	for (int i = 1; i < p->children_num; i++) {
		if (p->children[i]->visit > p->children[M]->visit) {
			M = i;
		}
	}

	return p->children[M];
}

MCTSSolver::Node * MCTSSolver::find_most_rated_node(Node * p) {
	int M = -INF;
	int M_idx = 0;

	for (int i = 0; i < p->children_num; i++) {
		Node * n = p->children[i];

		if (1.0 * n->win / n->visit > M) {
			M = 1.0 * n->win / n->visit;
			M_idx = i;
		}
	}

	return p->children[M_idx];
}

int MCTSSolver::solve(const Board & board, int dice, Info * info) {
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

				int winner = BoardUtil::winner(b);
				double val;

				if (winner == 1) {
					val = 1;
				}
				else if (winner = 2) {
					val = 0;
				}
				else {
					Node * node = new Node;
					node->board = b.flip();
					node->player = 1;
					expand(node);

					val = 1 - uct(node, 330000);

					del_nodes(node);
				}

				
				if (val > max_val) {
					max_val = val;
					best_move = live_piece_pos[i][0] * 100 + live_piece_pos[i][1] * 10 + dir;
				}
			}
		}
	}

	if (info) {
		info->rate = max_val;
		//info->extra = get_extra(best);
	}

	return best_move;
}

double MCTSSolver::uct(Node * root, int amount) {
	while (root->visit < amount) {
		Node * n = root;

		while (n->children_num) {
			n = select(n);
		}

		if (n->visit < MAX_SIM) {
			int win;
			if (n->winner == 0) {
				win = Simulator::simulate(n->board, 3 - n->player, TIMES_PER_SIM);
			}
			else {
				win = n->winner == 1 ? TIMES_PER_SIM : 0;
			}

			updateMCTS(n, win);
			//updateAMAF(n, win);
		}
		else {
			if (n->winner != 0) {
				int win = n->winner == 1 ? TIMES_PER_SIM : 0;
				updateMCTS(n, win);
				//updateAMAF(n, win);
			}
			else {
				expand(n);
				n = select(n);

				int win = 0;
				if (n->winner != 0) {
					win = n->winner == 1 ? TIMES_PER_SIM : 0;
				}
				else {
					win = Simulator::simulate(n->board, 3 - n->player, TIMES_PER_SIM);
				}
				
				updateMCTS(n, win);
				//updateAMAF(n, win);
			}
		}
	}

	return 1.0 * root->win / root->visit;
}

void MCTSSolver::expand(Node * rt) {
	int player = 3 - rt->player;
	int live_piece[7] = { 0 };
	int live_piece_pos[7][2] = { 0 };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = rt->board.get(i, j);
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

	for (int i = 1; i <= 6; i++) {
		if (live_piece[i]) {
			int l = player == 1 ? 4 : 0;
			int r = player == 1 ? 6 : 2;

			for (int dir = l; dir <= r; dir++) {
				int nx = live_piece_pos[i][0] + DIR[dir][0];
				int ny = live_piece_pos[i][1] + DIR[dir][1];

				if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
					Node * n = new Node;

					n->board = rt->board;
					n->board.move(live_piece_pos[i][0], live_piece_pos[i][1], dir);
					n->move = live_piece_pos[i][0] * 100 + live_piece_pos[i][1] * 10 + dir;
					//n->p = weight[i] / 6.0;
					n->player = player;
					n->parent = rt;
					n->winner = BoardUtil::winner(n->board);
					n->move_piece = i;
					
					rt->children[rt->children_num] = n;
					rt->children_num++;
				}
			}
		}
	}
}

//void MCTSSolver::expand_with_dice(Node * rt, int dice) {
//	int player = 3 - rt->player;
//	int live_piece[7] = { 0 };
//	int live_piece_pos[7][2] = { 0 };
//
//	for (int i = 0; i < 5; i++) {
//		for (int j = 0; j < 5; j++) {
//			int val = rt->board.get(i, j);
//			if (val > 0 && player == 1) {
//				live_piece[val] = 1;
//				live_piece_pos[val][0] = i;
//				live_piece_pos[val][1] = j;
//			}
//			else if (val < 0 && player == 2) {
//				live_piece[-val] = 1;
//				live_piece_pos[-val][0] = i;
//				live_piece_pos[-val][1] = j;
//			}
//		}
//	}
//
//	int i;
//	for (i = dice; i >= 1 && live_piece[i] == 0; i--);
//	if (i >= 1) {
//		live_piece[i] = 2;
//	}
//	for (i = dice; i <= 6 && live_piece[i] == 0; i++);
//	if (i <= 6) {
//		live_piece[i] = 2;
//	}
//
//	for (int i = 1; i <= 6; i++) {
//		if (live_piece[i] == 2) {
//			int l = player == 1 ? 4 : 0;
//			int r = player == 1 ? 6 : 2;
//
//			for (int dir = l; dir <= r; dir++) {
//				int nx = live_piece_pos[i][0] + DIR[dir][0];
//				int ny = live_piece_pos[i][1] + DIR[dir][1];
//
//				if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
//					Node * n = new Node;
//
//					n->board = rt->board;
//					n->board.move(live_piece_pos[i][0], live_piece_pos[i][1], dir);
//					n->move = live_piece_pos[i][0] * 100 + live_piece_pos[i][1] * 10 + dir;
//					n->p = 1;
//					n->player = player;
//					n->parent = rt;
//					n->winner = BoardUtil::winner(n->board);
//					n->move_piece = i;
//
//					rt->children[rt->children_num] = n;
//					rt->children_num++;
//				}
//			}
//		}
//	}
//}

MCTSSolver::Node * MCTSSolver::select(Node * rt) {
	int player = 3 - rt->player;
	int dice = MyRand::getIns()->rand_dice();
	int live_piece[7] = { 0 };
	int live_piece_pos[7][2] = { 0 };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = rt->board.get(i, j);
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
	int it;
	for (it = dice; it >= 1 && live_piece[it] == 0; it--);
	if (it >= 1) {
		live_piece[it] = 2;
	}
	for (it = dice; it <= 6 && live_piece[it] == 0; it++);
	if (it <= 6) {
		live_piece[it] = 2;
	}

	int M = 0;
	double M_score = -INF;
	for (int i = 0; i < rt->children_num; i++) {
		Node * n = rt->children[i];

		if (live_piece[n->move_piece] != 2)
			continue;

		if (n->visit < MIN_SIM) {
			return n;
		}

		//double beta = sqrt(AMAFK / (3 * n->visit + AMAFK));
		double ucb = UCBK * sqrt(log(n->parent->visit) / n->visit);

		/*n->score =
			(1 - beta) * (n->win / n->visit + ucb) +
			beta * rt->AMAFWin[n->move] / rt->AMAFVis[n->move];*/
		
		n->score = 1.0 * n->win / n->visit + ucb;
		if (n->score > M_score) {
			M_score = n->score;
			M = i;
		}
	}

	return rt->children[M];
}

void MCTSSolver::updateMCTS(Node * leaf, int win_count) {
	Node * n = leaf;

	while (n->parent) {
		n->visit += TIMES_PER_SIM;
		if (n->player == 1) {
			n->win += win_count;
		}
		else {
			n->win += TIMES_PER_SIM - win_count;
		}

		n = n->parent;
	}

	n->win += n->player == 1 ? win_count : TIMES_PER_SIM - win_count;
	n->visit += TIMES_PER_SIM;
}

void MCTSSolver::del_nodes(Node * rt) {
	for (int i = 0; i < rt->children_num; i++) {
		del_nodes(rt->children[i]);
	}

	delete rt;
}

void MCTSSolver::updateAMAF(Node * leaf, int win_count) {
	vector<pair<int, int>> moves;

	moves.push_back(make_pair(leaf->move, leaf->player));

	Node * n = leaf->parent;

	while (1) {
		for (auto i : moves) {
			if (i.second == 3 - n->player) {
				n->AMAFVis[i.first] += TIMES_PER_SIM;
				n->AMAFWin[i.first] += i.second == 1 ? win_count : TIMES_PER_SIM - win_count;
			}
		}

		moves.push_back(make_pair(n->move, n->player));

		if (n->parent) {
			n = n->parent;
		}
		else {
			break;
		}
	}


}

string MCTSSolver::get_extra(Node * best) {
	string ans = "";

	Node * n = best;
	int it = 1;
	while (1) {
		char temp[100];

		sprintf(temp, "%d: %d\n", it, n->move);

		ans += temp;
		it++;

		if (n->children_num) {
			n = find_most_visited_node(n);
		}
		else {
			break;
		}
	}

	return ans;
}