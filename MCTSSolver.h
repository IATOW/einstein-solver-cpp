#pragma once

class MCTSSolver {
public:
	struct Node {
		int player = 0;
		int winner = 0;
		int move = 0;
		int win = 0;
		int visit = 0;
		int move_piece = 0;

		double score = 0;

		Board board;

		Node * children[20];
		int children_num = 0;

		Node * parent = nullptr;

		std::map<int, int> AMAFWin;
		std::map<int, int> AMAFVis;
	};

	struct Info {
		double rate;
		std::string extra;
	};
private:

	static double uct(Node * rt, int amount);
	static void expand(Node * rt);
	//static void expand_with_dice(Node * rt, int dice);
	static Node * select(Node * rt);
	static void del_nodes(Node * rt);

	static void updateMCTS(Node * leaf, int win_count);
	static void updateAMAF(Node * leaf, int win_count);

	static Node * find_most_visited_node(Node * p);
	static Node * find_most_rated_node(Node * p);

	static std::string get_extra(Node * best);

public:
	static int solve(const Board & board, int dice, Info * info);
};