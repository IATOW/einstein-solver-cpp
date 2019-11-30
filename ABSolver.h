#pragma once

class ABSolver {
private:
	static double alphabeta(Board & board, double alpha, double beta, int dep, int player);
	static double naive_search(const Board & board, int dep, int player);

public:

	struct Info {
		double rate;
	};

	static int solve(const Board & board, int dice, Info * info);
};