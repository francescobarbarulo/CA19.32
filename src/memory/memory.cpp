#include "memory.h"

Memory::Memory(string name, int priority) : module(name, priority){
    dram = new uint32_t[DRAM_SIZE];
    first_read = true;
}

Memory::Memory(string name, int priority, ModeType mode, Bus *bus) {
    this.mode = mode;
    this.bus = bus;
}

bool Memory::isSelfMessage(message* m){
    return (m->source == getName());
}

// it should be the method called upon the message receiving?
void Memory::onNotify(message *msg){
    if (msg->dest == this->getName()){
        int dram_access_time = 10;
        // first of all get the current bus status
        bus->get(&this->bus_status);

        // it is from me after a certain memory access time
        if (bus_status.request == READ){
            // read access
            // bus_status setup
            int cell_address = bus_status.address >> 2;
            bus_status.data = dram[cell_address];
            if (bus->set(&bus_status)){
                cout << "[I] bus set successfully" << endl;
                // Can I delete the received message?
                // response creation
                message *response = new message();
                response->valid = 1;
        		response->timestamp = getTime();
        		strcpy(response->source, getName().c_str());
        		strcpy(response->dest, msg->source);
        		response->magic_struct = 0;

                /* compute the access time
                if (mode == DEFAULT){
                    dram_access_time = defaultBehavior(); // CL+RCD+RP
                } else {
                    dram_access_time = fastBehavior(); // it depends
                }*/

                sendWithDelay(response, dram_access_time);


            } else {
                // !! what to do ?!?!
                cout << "[E] error on bus setting" << endl;
            }
        } else {
            // write access
            dram[bus_status.address] = bus_status.data;
        }
    }
}
