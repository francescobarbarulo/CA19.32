#include "memory.h"

Memory::Memory(string name, int priority, Bus *bus, string program) : module(name, priority) {
    dram = new uint32_t[DRAM_SIZE];
    first_access = true;
    refreshing_phase = false;
    this->bus = bus;

    // load program
    ifstream file;
    file.open(program, ios::in | ios::binary | ios::ate);
    if (file.is_open()){
        int size = file.tellg();
        file.seekg(0, file.beg);

        file.read((char*)dram, size);
        file.close();
        cout << "[I] Program '" << program << "' loaded" << endl;
    }

    // message for memory refresh
    if (REFRESH_EN){
        message *refresh_msg = createMessage(getName(), getName());
        sendWithDelay(refresh_msg, REFRESHING_INTERVAL);
    }
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

    return time;
}

void Memory::startRefresh(){
    refreshing_phase = true;
    refreshing_phase_started_at = getTime();

    message *refresh_ending_msg = createMessage(getName(), getName());
    sendWithDelay(refresh_ending_msg, REFRESHING_TIME);
    cout << dec << getTime() << " [" << getName() << "] Refreshing..." << endl;
}

void Memory::endRefresh(){
    refreshing_phase = false;
    message *refresh_msg = createMessage(getName(), getName());
    sendWithDelay(refresh_msg, REFRESHING_INTERVAL);
    cout << dec << getTime() << " [" << getName() << "] Refreshing phase ended" << endl;
}

void Memory::onReadRequest(uint16_t cell_address, uint16_t current_row_addressed, string response_dest){
    int dram_access_time = 0;
    cout << dec << getTime() << " [" << getName() << "] Received read request at cell " << cell_address << endl;

    bus_status.data = dram[cell_address];

    if ( bus->set(&bus_status) ){
        // compute the access time
        if (MODE_TYPE == DEFAULT){
            dram_access_time = defaultBehavior();
        } else {
            dram_access_time = optimizedBehavior(current_row_addressed);
        }

        // response msg creation
        message *response = createMessage(getName(), response_dest);
        sendWithDelay(response, dram_access_time);
        cout << dec << getTime() << " [" << getName() << "] Sending msg to " << response_dest << " with delay of " << dram_access_time << endl;

    } else {
        cout << dec << "[" << getName() << "] Fail accessing the bus" << endl;
        cerr << "[ERR] Something went wrong in the protocol" << endl;
        exit(EXIT_FAILURE);
    }
}

void Memory::onWriteRequest(uint16_t cell_address){
    cout << dec << getTime() << " [" << getName() << "] Received write request at cell " << cell_address << endl;
    dram[cell_address] = bus_status.data;
}

void Memory::onNotify(message *msg){
    if ( msg->dest == this->getName() ){

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

            // first of all get the current bus status
            if ( !bus->get(&bus_status) ){
                cout << dec << "[" << getName() << "] Fail accessing the bus" << endl;
                return;
            }

            uint16_t cell_address = bus_status.address >> 2;
            uint16_t current_row_addressed = cell_address >> 8;

            if ( bus_status.request == READ ){
                onReadRequest(cell_address, current_row_addressed, string(msg->source));
            } else if ( bus_status.request == WRITE ) {
                onWriteRequest(cell_address);
            } else {
                cerr << "[ERR] Undefined bus status" << endl;
                exit(EXIT_FAILURE);
            }

            last_row_addressed = current_row_addressed;
            // from now on there will be normal accesses according to dram response times
            if (first_access) { first_access = !first_access; }
        }
    }
}
