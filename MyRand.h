#pragma once

class MyRand {
private:
	std::default_random_engine e;
public:
	MyRand();

	static MyRand * getIns();

	int rand_dice();

	int rand_between(int l, int r);
};