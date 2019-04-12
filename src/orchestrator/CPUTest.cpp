#include <cstring>
#include <iostream>

#include "CPUTest.h"

using namespace std;

CPUTest::CPUTest(string name, int priority, Bus *bus) : module(name, priority) {
	// init test addresses for write/read operations
	for (int i = 0; i < N_OPERATIONS; i++){
		test_addresses.push_back(i * 123);
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

	cout << getTime() << " [" << getName() << "] Sending write request: " << bus_status.data << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 10);

	op_counter++;

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

	cout << getTime() << " [" << getName() << "] Sending read request" << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 10);

	op_counter++;

	return true;
}

//All custom code must be called from this method
/*
void CPUTest::onNotify(message* m) {
	if ( m->dest == getName() ){

		switch (current_status) {
			case WRITING:
				if (isSelfMessage(m)) {
					if (write_request()) current_status = IDLE;
				}
			break;
			case READING:
				if (isSelfMessage(m)) {
					if (read_request()) current_status = IDLE;
				}
			break;
			case IDLE:
				if (isSelfMessage(m)) {
					if (getTime() % 17 == 0) {
						current_status = WRITING;
					}
					else if (getTime() % 31 == 0) {
						current_status = READING;
					}
				} else {
					// read from bus and print data
					if ( bus->get(&bus_status) ){
						cout << "[" << getName() << "] read done: " << bus_status.data << endl;
						current_status = IDLE;
					}
				}


				if ( (getTime() + 10) % 20 == 0 ) {
					read_request();
				} else if ( getTime() % 20 == 0 ) {
					write_request();
				}

			break;
		}

		if ( getTime() < 100 ){
			message *beep = createMessage(getName());
			sendWithDelay(beep, 1);
		}
	}
}
*/

void CPUTest::onNotify(message *m){
	if (m->dest == getName()){

		if (op_counter > N_OPERATIONS * 2){ return; }

		if (!isSelfMessage(m)){
			if ( bus->get(&bus_status) ){
				cout << "[" << getName() << "] Read " << bus_status.data << endl;
			}
		}

		if (op_counter < N_OPERATIONS ){
			write_request();
			message *beep = createMessage(getName());
			sendWithDelay(beep, 10);
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
