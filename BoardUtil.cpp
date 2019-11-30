#include "INCLUDE.h"

using std::vector;

int BoardUtil::winner(const Board & board) {
	if (board.get(0, 0) < 0) {
		return 2;
	}

	if (board.get(4, 4) > 0) {
		return 1;
	}

	if (board.get_blue_remain() == 0) {
		return 1;
	}

	if (board.get_red_remain() == 0) {
		return 2;
	}

	return 0;
}

//int BoardUtil::valid_piece(const Board & board, int dice, int player) {
//	int ans = 0;
//
//	int temp[7] = { 0 };
//
//	for (int i = 0; i < 5; i++) {
//		for (int j = 0; j < 5; j++) {
//			if (player == 1) {
//				if (board.get(i, j) > 0) {
//					temp[board.get(i, j)] = 1;
//				}
//			}
//			else if (player == 2) {
//				if (board.get(i, j) < 0) {
//					temp[-board.get(i, j)] = 1;
//				}
//			}
//		}
//	}
//
//	if (temp[dice] == 1) {
//		return dice;
//	}
//
//	int i;
//	for (i = dice; i >= 1 && temp[i] == 0; i--);
//	if (i != 0) {
//		ans = i;
//	}
//
//	for (i = dice; i <= 6 && temp[i] == 0; i++);
//	if (i != 7) {
//		ans = ans * 10 + i;
//	}
//
//	return ans;
//}

void BoardUtil::valid_move(const Board & board, int dice, int player, vector<int> & out) {
	out.clear();

	int temp[7] = { 0 };
	int temp_pos[7][2] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val = board.get(i, j);

			if (val > 0 && player == 1) {
				temp[val] = 1;
				temp_pos[val][0] = i;
				temp_pos[val][1] = j;
			}
			else if (val < 0 && player == 2) {
				temp[-val] = 1;
				temp_pos[-val][0] = i;
				temp_pos[-val][1] = j;
			}
		}
	}

	if (temp[dice] == 1) {
		int l = player == 1 ? 4 : 0;
		int r = player == 1 ? 6 : 2;
		for (int i = l; i <= r; i++) {
			int nx = temp_pos[dice][0] + DIR[i][0];
			int ny = temp_pos[dice][1] + DIR[i][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				out.push_back(temp_pos[dice][0] * 100 + temp_pos[dice][1] * 10 + i);
			}
		}

		return;
	}

	int it;
	for (it = dice - 1; it >= 1 && temp[it] == 0; it--);
	if (it >= 1) {
		int l = player == 1 ? 4 : 0;
		int r = player == 1 ? 6 : 2;
		for (int i = l; i <= r; i++) {
			int nx = temp_pos[it][0] + DIR[i][0];
			int ny = temp_pos[it][1] + DIR[i][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				out.push_back(temp_pos[it][0] * 100 + temp_pos[it][1] * 10 + i);
			}
		}
	}

	for (it = dice + 1; it <= 6 && temp[it] == 0; it++);
	if (it <= 6) {
		int l = player == 1 ? 4 : 0;
		int r = player == 1 ? 6 : 2;
		for (int i = l; i <= r; i++) {
			int nx = temp_pos[it][0] + DIR[i][0];
			int ny = temp_pos[it][1] + DIR[i][1];

			if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
				out.push_back(temp_pos[it][0] * 100 + temp_pos[it][1] * 10 + i);
			}
		}
	}

}

//void BoardUtil::valid_move_one_piece(const Board & board, int piece, int player, vector<int> & out) {
//	if (player == 1) {
//
//	}
//}

void BoardUtil::set_layout(Board & board, int player, const char * layout) {
	if (player == 1) {
		board.set(0, 0, layout[0] - '0');
		board.set(0, 1, layout[1] - '0');
		board.set(0, 2, layout[2] - '0');
		board.set(1, 0, layout[3] - '0');
		board.set(1, 1, layout[4] - '0');
		board.set(2, 0, layout[5] - '0');
	}
	else if (player == 2) {
		board.set(4, 4, '0' - layout[0]);
		board.set(4, 3, '0' - layout[1]);
		board.set(4, 2, '0' - layout[2]);
		board.set(3, 4, '0' - layout[3]);
		board.set(3, 3, '0' - layout[4]);
		board.set(2, 4, '0' - layout[5]);
	}
}

void BoardUtil::print_board(const Board & board) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%3d", board.get(i, j));
		}
		printf("\n");
	}
}

void BoardUtil::flip(int & x, int & y) {
	x = 4 - x;
	y = 4 - y;
}