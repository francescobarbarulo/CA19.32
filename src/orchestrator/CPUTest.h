#pragma once
#include <string>
#include <vector>
#include <unistd.h>

#include "module.h"
#include "structures.h"
#include "../bus/bus.h"

#define N_OPERATIONS 5

class CPUTest : public module
{
	// test var
	vector<uint16_t> test_addresses;
	unsigned int op_counter;

	Bus_status bus_status;
	Bus *bus;
public:
	CPUTest(string, int, Bus*);
	void onNotify(message*);
	message* createMessage(string);
	bool isSelfMessage(message*);
	bool write_request(bool);
	bool read_request(bool);
};
