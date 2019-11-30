#include "INCLUDE.h"

MyRand::MyRand() {
	e.seed(time(0));
}

MyRand * MyRand::getIns() {
	static MyRand ins;

	return &ins;
}

int MyRand::rand_dice() {
	return e() % 6 + 1;
}

int MyRand::rand_between(int l, int r) {
	return e() % (r - l + 1) + l;
}