#include <cstring>
#include <iostream>

#include "CPUTest.h"

using namespace std;

CPUTest::CPUTest(string name, int priority, Bus *bus) : module(name, priority) {
	srand(7); // 7 🤴 kings of Rome
	// init test addresses for write/read operations
	if (ACCESS_TYPE == SEQUENTIAL){
		for (int i = 0; i < N_OPERATIONS; i++){
			test_addresses.push_back(i);
		}
	} else if (ACCESS_TYPE == RANDOM) {
		for (int i = 0; i < N_OPERATIONS; i++){
			test_addresses.push_back(rand() % MEM_SIZE);
		}
	} else {
		for (int i = 0; i < N_OPERATIONS; i++){
			if (rand() % 100 > PROB)
				test_addresses.push_back(rand() % MEM_SIZE);
			else
				test_addresses.push_back(i);
		}
	}
	// init main bus
	this->bus = bus;
	// auto-message to start the system
	message* myMessage = createMessage(getName());
	sendWithDelay(myMessage, 0);
}

bool CPUTest::isSelfMessage(message *m){
	return (m->source == getName());
}

bool CPUTest::write_request(){
	string dest = "MEM";

	bus_status.request = WRITE;
	bus_status.address = test_addresses[op_counter % N_OPERATIONS];
	bus_status.data = test_addresses[op_counter] + 1;

	if ( !bus->set(&bus_status) ){
		cout << getTime() << " [" << getName() << "] Fail accessing the bus on write" << endl;
		return false;
	}

	op_counter++;

	cout << getTime() << " [" << getName() << "] Sending write request at address "<< bus_status.address << ": " << bus_status.data << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 1);

	return true;
}

bool CPUTest::read_request(){
	string dest = "MEM";

	bus_status.request = READ;
	bus_status.address = test_addresses[op_counter % N_OPERATIONS];

	if ( !bus->set(&bus_status) ){
		cout << getTime() << " [" << getName() << "] Fail accessing the bus on read" << endl;
		return false;
	}

	op_counter++;

	cout << getTime() << " [" << getName() << "] Sending read request at address " << bus_status.address << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 1);

	return true;
}

void CPUTest::onNotify(message *m){
	if (m->dest == getName()){
		// just to update the output every second
		//sleep(1);

		if (!isSelfMessage(m)){
			if ( bus->get(&bus_status) ){
				cout << getTime() << " [" << getName() << "] Read " << bus_status.data << endl;
			}
		}

		if (op_counter >= N_OPERATIONS * 2){
			cout << "Shutdown" << endl;
			exit(0);
		}

		if (op_counter < N_OPERATIONS ){
			write_request();
			message *beep = createMessage(getName());
			sendWithDelay(beep, 1);
		} else {
			read_request();
		}
	}
}

message* CPUTest::createMessage(string dest){
	message *msg = new message();
	msg->valid = 1;
	msg->timestamp = getTime();
	strcpy(msg->source, getName().c_str());
	strcpy(msg->dest, dest.c_str());
	msg->magic_struct = NULL;

	return msg;
}
