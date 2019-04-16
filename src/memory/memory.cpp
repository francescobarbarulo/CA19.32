#include "memory.h"

Memory::Memory(string name, int priority, Bus *bus) : module(name, priority) {
    dram = new uint32_t[DRAM_SIZE];
    first_access = true;
    this->bus = bus;

    // message for memory refresh
    message *refresh_msg = createMessage(getName(), getName());
    sendWithDelay(refresh_msg, REFRESHING_INTERVAL);
}

bool Memory::isSelfMessage(message *m){
    return (m->source == getName());
}

message* Memory::createMessage(string src, string dest){
	message *msg = new message();
	msg->valid = 1;
	msg->timestamp = getTime();
	strcpy(msg->source, src.c_str());
	strcpy(msg->dest, dest.c_str());
	msg->magic_struct = NULL;

	return msg;
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
    refreshing_phase_started_at = getTime();

    message *refresh_ending_msg = createMessage(getName(), getName());
    sendWithDelay(refresh_ending_msg, REFRESHING_TIME);
    cout << getTime() << " [" << getName() << "] Refreshing..." << endl;
}

void Memory::endRefresh(){
    refreshing_phase = false;
    message *refresh_msg = createMessage(getName(), getName());
    sendWithDelay(refresh_msg, REFRESHING_INTERVAL);
    cout << getTime() << " [" << getName() << "] Refreshing phase ended" << endl;
}

void Memory::onNotify(message *msg){
    if ( msg->dest == this->getName() ){
        // just to update the output every second
        sleep(1);
        
        if (isSelfMessage(msg)){
            // for refreshing phase
            if (refreshing_phase){ endRefresh(); }
            else { startRefresh(); }

        } else {
            if (refreshing_phase){
                // Postpone the request at the end of refreshing phase
                message *retry_msg = createMessage(string(msg->source), getName());
                sendWithDelay(retry_msg, refreshing_phase_started_at + REFRESHING_TIME - getTime());
                return;
            }

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
                        dram_access_time = defaultBehavior();
                    } else {
                        dram_access_time = optimizedBehavior(cell_address >> 8);
                    }

                    // response msg creation
                    message *response = createMessage(getName(), (string)msg->source);
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
    }
}
