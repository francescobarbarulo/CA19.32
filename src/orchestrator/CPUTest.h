#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

#include "module.h"
#include "structures.h"
#include "../bus/bus.h"
#include "parameters.h"

using namespace std;

#define MEM_SIZE 64 * (1 << 10)

struct access_request {
	request_t request;
	uint16_t address;
};

// sim params
#define N_OPERATIONS MEM_SIZE >> 1
extern Parameters params;

class CPUTest : public module {
	// params
	AccessType ACCESS_TYPE;
	unsigned int RANDOM_ACCESS_PROB;
	unsigned int READ_REQ_PROB;
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
