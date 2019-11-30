#pragma once

class BoardUtil {
public:
	static int winner(const Board & board);

	//static int valid_piece(const Board & board, int dice, int player);

	static void valid_move(const Board & board, int dice, int player, std::vector<int> & out);

	//static void valid_move_one_piece(const Board & board, int piece, int player, std::vector<int> & out);

	static void set_layout(Board & board, int player, const char * layout);

	static void print_board(const Board & board);

	static void flip(int & x, int & y);
};