#include "INCLUDE.h"

using std::vector;

int Simulator::simulate(const Board & board, int player, int num) {
	int ans = 0;

	for (int i = 0; i < num; i++) {
		// copy the board
		Board b = board;
		int turn = player;
		int w;

		while (1) {
			int dice = MyRand::getIns()->rand_dice();

			vector<int> valid_move;
			BoardUtil::valid_move(b, dice, turn, valid_move);

			int idx = MyRand::getIns()->rand_between(0, valid_move.size() - 1);
			int action = valid_move[idx];

			int x = action / 100;
			int y = action / 10 % 10;
			int dir = action % 10;

			b.move(x, y, dir);


			w = BoardUtil::winner(b);
			if (w) {
				break;
			}

			turn = 3 - turn;
		}

		if (w == 1) {
			ans++;
		}
	}

	return ans;
}