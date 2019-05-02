#pragma once
#include <string>
#include <vector>
#include <unistd.h>

#include "module.h"
#include "structures.h"
#include "../bus/bus.h"

#define MEM_SIZE 64 * (1 << 10)

enum AccessType {
	SEQUENTIAL, RANDOM, STOCHASTIC, REAL
};
// sim params
#define N_OPERATIONS	1000
#define ACCESS_TYPE	STOCHASTIC
#define PROB 		80	//TODO: Check the possible real probability for sequential operations

class CPUTest : public module {
	// test variables
	vector<uint16_t> test_addresses;
	unsigned int op_counter;

	Bus_status bus_status;
	Bus *bus;
public:
	CPUTest(string, int, Bus*);
	void onNotify(message*);
	message* createMessage(string);
	bool isSelfMessage(message*);
	bool write_request();
	bool read_request();
};
