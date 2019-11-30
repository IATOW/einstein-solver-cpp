#include "INCLUDE.h"

using std::string;
using std::ifstream;

Board::Board() {
	memset(data, 0, sizeof(data));
	red_remain = 6;
	blue_remain = 6;
}

Board::Board(const Board & board) {
	memcpy(this->data, board.data, sizeof(board.data));
	this->red_remain = board.red_remain;
	this->blue_remain = board.blue_remain;
}

int Board::get(int x, int y) const {
	if (x < 0 || x >= 5 || y < 0 || y >= 5) {
		throw std::exception();
	}

	return data[x][y];
}

int Board::get(int pos) const {
	return get(pos / 5, pos % 5);
}

void Board::move(int x, int y, int dir) {
	int nx = x + DIR[dir][0];
	int ny = y + DIR[dir][1];

	if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) {
		throw std::exception();
	}

	int last = data[x][y];
	data[x][y] = 0;
	if (data[nx][ny] > 0) {
		red_remain--;
	}
	else if (data[nx][ny] < 0) {
		blue_remain--;
	}
	data[nx][ny] = last;
}

void Board::move(int pos, int dir) {
	move(pos / 5, pos % 5, dir);
}

int Board::get_blue_remain() const {
	return blue_remain;
}

int Board::get_red_remain() const {
	return red_remain;
}

void Board::set(int x, int y, int val) {
	data[x][y] = val;
}

void Board::set(int pos, int val) {
	set(pos / 5, pos % 5, val);
}

Board Board::flip() const {
	Board ans = *this;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			ans.set(4 - i, 4 - j, -data[i][j]);
		}
	}

	std::swap(ans.red_remain, ans.blue_remain);

	return ans;
}

Board & Board::operator=(const Board & board) {
	memcpy(this->data, board.data, sizeof(board.data));
	this->blue_remain = board.blue_remain;
	this->red_remain = board.red_remain;

	return *this;
}

Board Board::load_from(const string & filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		printf("error");
		return Board();
	}

	Board ans;
	ans.blue_remain = 0;
	ans.red_remain = 0;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int val;

			file >> val;
			if (val > 0) {
				ans.red_remain++;
			}
			if (val < 0) {
				ans.blue_remain++;
			}

			ans.data[i][j] = val;
		}
	}

	return ans;
}

void Board::weight(const Board & board, int player, int out[7]) {
	memset(out, 0, sizeof(int) * 7);

	
}