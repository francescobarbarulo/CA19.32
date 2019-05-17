#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>
#include <stdlib.h>

using namespace std;

enum AccessType {
	STOCHASTIC, REAL
};

enum ModeType {
    DEFAULT, FAST
};

struct Parameters {
    AccessType cpu_access;
    ModeType memory_mode;
    unsigned int random_access_prob;
    unsigned int read_req_prob;
    unsigned int seed;
	string file;
};

extern Parameters params;

void parseCommand(int, char**);

#endif
