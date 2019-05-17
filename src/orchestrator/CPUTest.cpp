#include "CPUTest.h"

CPUTest::CPUTest(string name, int priority, Bus *bus) : module(name, priority) {
	// init params
	ACCESS_TYPE = params.cpu_access;
	RANDOM_ACCESS_PROB = params.random_access_prob;
	READ_REQ_PROB = params.read_req_prob;

	// init test addresses for write/read operations
	if (ACCESS_TYPE == STOCHASTIC) {
		mt19937 generator(params.seed);
		for (int i = 0; i < N_OPERATIONS; i++){
			access_request ar = access_request();
			ar.request = (generator() % 100 < READ_REQ_PROB) ? READ : WRITE;
			ar.address = (generator() % 100 < RANDOM_ACCESS_PROB) ? (generator() % MEM_SIZE) : i;
			test_requests.push_back(ar);
		}
	} else {
		string filename = "examples/" + params.file;
		ifstream file;
		string line;
		file.open(filename, ios::in);
		if (file.is_open()){
			string request;
			uint16_t address;
			int counter = 0;
			while(true) {
                //if (counter >= N_OPERATIONS)  break;
			 	file >> request;
				file >> hex >> address;

                if (! file) break;

				access_request ar = access_request();
				ar.request = (request == "R") ? READ : WRITE;
				ar.address = address;
				test_requests.push_back(ar);

				++counter;
			}
			file.close();
		} else {
			cerr << "Unable to open " << filename << endl;
		}

	}
	// init main bus
	this->bus = bus;
	// auto-message to start the system
	message* myMessage = createMessage(getName());
	sendWithDelay(myMessage, 0);
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

bool CPUTest::isSelfMessage(message *m){
	return (m->source == getName());
}

void CPUTest::sendSelfMessage(){
	message *beep = createMessage(getName());
	sendWithDelay(beep, 1);
}

void CPUTest::sendRequest(){
	string dest = "MEM";
	string request_str = (test_requests[op_counter].request == WRITE) ? "W" : "R";

	bus_status.request = test_requests[op_counter].request;
	bus_status.address = test_requests[op_counter].address;
	bus_status.data = 77;

	if ( !bus->set(&bus_status) ){
		/* It may happen only on write attempts because the CPU is notified by itself
		   and the memory may not have got the bus status. Thus the CPU will try to set
		   the bus until it succeeds
		*/
		clog << dec << getTime() << " [" << getName() << "] Fail accessing the bus on " << request_str << endl;
		sendSelfMessage();
		return;
	}

	clog << dec << getTime() << " [" << getName() << "] " << request_str << " ";
	clog << hex << "0x" << setfill('0') << setw(4) << bus_status.address << endl;
	message *request = createMessage(dest);
	sendWithDelay(request, 1);

	if (test_requests[op_counter].request == WRITE){
		// schedule next operation here because the write request does not get back any response
		sendSelfMessage();
	}

	// increment operation counter
	op_counter++;
}

void CPUTest::onNotify(message *m){
	if (m->dest == getName()){

		if (!isSelfMessage(m)){
			if ( bus->get(&bus_status) ){
				clog << dec << getTime() << " [" << getName() << "] Data ";
				clog << hex << bus_status.data << endl;
			}
		}

		if (op_counter >= test_requests.size()){
			cout << getTime() << endl;
			exit(0);
		}

		sendRequest();
	}
}
