#pragma once

/*
direction:
1 2 3
0   4
7 6 5
*/

/*
move:
ijk
(i, j) dir: k
*/

class Board {
private:
	int data[5][5];
	int red_remain;
	int blue_remain;

public:
	Board();
	Board(const Board & board);

	int get(int x, int y) const;
	int get(int pos) const;

	void set(int x, int y, int val);
	void set(int pos, int val);

	void move(int x, int y, int dir);
	void move(int pos, int dir);

	int get_blue_remain() const;
	int get_red_remain() const;

	Board flip() const;

	Board & operator=(const Board & board);

	static Board load_from(const std::string & filename);

	static void weight(const Board & board, int player, int out[7]);
};