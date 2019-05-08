#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "module.h"
#include "structures.h"
#include "../bus/bus.h"

using namespace std;

#define MEM_SIZE 64 * (1 << 10)

enum access_t {
	STOCHASTIC, REAL
};

struct access_request {
	request_t request;
	uint16_t address;
};

// sim params
#define N_OPERATIONS 		1000
#define ACCESS_TYPE			STOCHASTIC
#define RANDOM_ACCESS_PROB	0	// 0÷100%
#define READ_REQ_PROB 		0	// 0÷100%

class CPUTest : public module {
	// test variables
	vector<access_request> test_requests;
	unsigned int op_counter;

	Bus_status bus_status;
	Bus *bus;
public:
	CPUTest(string, int, Bus*);
	void onNotify(message*);
	message* createMessage(string);
	bool isSelfMessage(message*);
	void sendSelfMessage();
	void sendRequest();
};
