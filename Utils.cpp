#include "INCLUDE.h"

using std::string;
using std::swap;

string Utils::random_layout() {
	static const int seq[6] = {
		1,2,3,4,5,6
	};

	int temp[6];
	for (int i = 0; i < 6; i++) {
		temp[i] = seq[i];
	}

	string ans;

	for (int i = 0; i < 6; i++) {
		int idx = MyRand::getIns()->rand_between(0, 5 - i);
		ans.push_back(temp[idx] + '0');

		swap(temp[idx], temp[5 - i]);
	}

	return ans;
}