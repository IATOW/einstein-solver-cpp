#pragma once

const int INF = 0x3f3f3f3f;

const int DIR[8][2] = {
	0,-1,
	-1,-1,
	-1,0,
	-1,1,
	0,1,
	1,1,
	1,0,
	1,-1
};

const char LAYOUT[20] = "123456";
const char NAME[20] = "worm";

const int MIN_SIM = 5;
const int MAX_SIM = 10;
const int TIMES_PER_SIM = 1;
const double UCBK = 1.414;
const double AMAFK = 0;