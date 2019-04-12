#include "memory.h"

Memory::Memory(string name, int priority, Bus *bus) : module(name, priority) {
    dram = new uint32_t[DRAM_SIZE];
    first_read = true;
    this->bus = bus;
}

int Memory::defaultBehavior(){
    return CL + RCD + RP;
}

int Memory::optimizedBehavior(){
    return 0;
}

void Memory::onNotify(message *msg){
    if ( msg->dest == this->getName() ){
        int dram_access_time = 0;
        // first of all get the current bus status
        if ( !bus->get(&bus_status) ){
            cout << "[" << getName() << "] Fail accessing the bus" << endl;
            return;
        }

        uint16_t cell_address = bus_status.address >> 2;

        if ( bus_status.request == READ ){
            // read access
            cout << getTime() << " [" << getName() << "] Received read request at cell " << cell_address << endl;

            bus_status.data = dram[cell_address];

            if ( bus->set(&bus_status) ){
                // compute the access time
                if (mode == DEFAULT){
                    dram_access_time = defaultBehavior(); // CL+RCD+RP
                } else {
                    dram_access_time = optimizedBehavior(); // it depends
                }

                // response msg creation
                message *response = createMessage((string)msg->source);
                sendWithDelay(response, dram_access_time);
                cout << getTime() << " [" << getName() << "] message sent to " << msg->source << endl;

                // from now on there will be normal reads
                if (first_read) { first_read = !first_read; }

            } else {
                // !! what to do ?!?!
                cout << "[" << getName() << "] Fail accessing the bus" << endl;
            }
        } else if ( bus_status.request == WRITE ) {
            // write access
            cout << getTime() << " [" << getName() << "] Received write request at cell " << cell_address << endl;
            dram[cell_address] = bus_status.data;
        } else {
            return;
        }
    }
}

message* Memory::createMessage(string dest){
	message *msg = new message();
	msg->valid = 1;
	msg->timestamp = getTime();
	strcpy(msg->source, getName().c_str());
	strcpy(msg->dest, dest.c_str());
	msg->magic_struct = NULL;

	return msg;
}
