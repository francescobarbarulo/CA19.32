#include <cstring>
#include <iostream>

#include "CPUTest.h"

using namespace std;

CPUTest::CPUTest(string name, int priority, Bus *bus) : module(name, priority) {
	// init test addresses for write/read operations
	for (int i = 0; i < N_OPERATIONS; i++){
		test_addresses.push_back(i * 512);
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

bool CPUTest::write_request(bool new_request){
	string dest = "MEM";

	if (new_request){
		bus_status.request = WRITE;
		bus_status.address = test_addresses[op_counter % N_OPERATIONS];
		bus_status.data = test_addresses[op_counter] + 1;

		if ( !bus->set(&bus_status) ){
			cout << getTime() << " [" << getName() << "] Fail accessing the bus on write" << endl;
			return false;
		}

		op_counter++;
	}

	cout << getTime() << " [" << getName() << "] Sending write request at address "<< bus_status.address << ": " << bus_status.data << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 10);

	return true;
}

bool CPUTest::read_request(bool new_request){
	string dest = "MEM";

	if (new_request){
		bus_status.request = READ;
		bus_status.address = test_addresses[op_counter % N_OPERATIONS];

		if ( !bus->set(&bus_status) ){
			cout << getTime() << " [" << getName() << "] Fail accessing the bus on read" << endl;
			return false;
		}

		op_counter++;
	}

	cout << getTime() << " [" << getName() << "] Sending read request at address " << bus_status.address << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 10);

	return true;
}

void CPUTest::onNotify(message *m){
	if (m->dest == getName()){
		sleep(1);

		bool valid = true;

		if (!isSelfMessage(m)){
			valid = (getTime() != m->timestamp);
			if (valid){
				cout << getTime() << " [" << getName() << "] Received valid message" << endl;
			} else {
				cout << getTime() << " [" << getName() << "] Received invalid message" << endl;
			}

			if ( valid && bus->get(&bus_status) ){
				cout << getTime() << " [" << getName() << "] Read " << bus_status.data << endl;
			}
		}

		if (op_counter >= N_OPERATIONS * 2){ return; }

		if (op_counter < N_OPERATIONS ){
			write_request(valid);
			message *beep = createMessage(getName());
			sendWithDelay(beep, 10);
		} else {
			read_request(valid);
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
