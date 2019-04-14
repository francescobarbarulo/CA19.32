#include "memory.h"

Memory::Memory(string name, int priority, Bus *bus) : module(name, priority) {
    dram = new uint32_t[DRAM_SIZE];
    first_access = true;
    this->bus = bus;

    // message for memory refresh
    message *refresh_msg = createMessage(getName());
    sendWithDelay(refresh_msg, REFRESH_INTERVAL);
}

int Memory::defaultBehavior(){
    return CL + RCD;
}

int Memory::optimizedBehavior(uint16_t current_row_addressed){
    unsigned int time;

    if (first_access){
        time = CL + RCD;
    } else if ( current_row_addressed == last_row_addressed ){
        time = CL;
    } else {
        time = CL + RCD + RP;
    }

    last_row_addressed = current_row_addressed;

    return time;
}

void Memory::startRefresh(){
    refreshing_phase = true;
    message *refresh_ending_msg = createMessage(getName());
    sendWithDelay(refresh_ending_msg, RCD);
    cout << getTime() << " [" << getName() << "] Refreshing..." << endl;
}

void Memory::endRefresh(){
    refreshing_phase = false;
    message *refresh_msg = createMessage(getName());
    sendWithDelay(refresh_msg, REFRESH_INTERVAL);
    cout << getTime() << " [" << getName() << "] Refreshing phase ended" << endl;
}

void Memory::onNotify(message *msg){
    if ( msg->dest == this->getName() ){
        sleep(1);
        if (isSelfMessage(msg)){
            // for refreshing phase
            if (refreshing_phase){
                endRefresh();
            } else {
                startRefresh();
            }
        } else {
            int dram_access_time = 0;
            /*
            if (refreshing_phase){
                message *busy_msg = createMessage((string)msg->source);
                sendWithDelay(busy_msg, 0);
                cout << getTime() << " [" << getName() << "] Sending invalid message to " <<  msg->source << endl;
                return;
            }*/
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
                        dram_access_time = defaultBehavior();
                    } else {
                        dram_access_time = optimizedBehavior(cell_address >> 8);
                    }

                    // response msg creation
                    message *response = createMessage((string)msg->source);
                    sendWithDelay(response, dram_access_time);
                    cout << getTime() << " [" << getName() << "] sending msg to " << msg->source << " with delay of " << dram_access_time << endl;

                    // from now on there will be normal reads
                    if (first_access) { first_access = !first_access; }

                } else {
                    // !! what to do ?!?!
                    cout << "[" << getName() << "] Fail accessing the bus" << endl;
                }
            } else if ( bus_status.request == WRITE ) {
                // write access
                cout << getTime() << " [" << getName() << "] Received write request at cell " << cell_address << endl;
                dram[cell_address] = bus_status.data;
            }
        }
        // message consumed
        //delete(msg);
    }
}

bool Memory::isSelfMessage(message *m){
    return (m->source == getName());
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
