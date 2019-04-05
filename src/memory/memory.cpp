#include "memory.h"

Memory::Memory(){
    dram = new uint32_t[DRAM_SIZE];
    first_read = true;
}

Memory::Memory(ModeType mode, Bus *bus){
    this.mode = mode;
    this.bus = bus;
}

bool Memory::isSelfMessage(message *msg){
    return (strcmp(msg->source, msg->dest) == 0);
}

// it should be the method called upon the message receiving?
void Memory::handleMessage(message *msg){
    if (isSelfMessage(msg)){
        // it is from me after a certain memory access time
        if (bus_status.request == READ){
            // bus_status setup
            bus_status.data = dram[bus_status.address];
            if (bus->set(&bus_status)){
                cout << "[i] bus set successfully" << endl;
                // Can I delete the received message?
                // response creation
                message *response = new message();
                strcpy(response->dest, src_module);
                strcpy(response->source, msg->dest);
                notify(response);
            } else {
                // !! what to do ?!?!
                cout << "[i] error on bus setting" << endl;
            }
        } else {
            dram[bus_status.address] = bus_status.data;
        }
    } else {
        int dram_access_time = 0;
        // it is from someone and I save the source for the read request response
        strcpy(src_module, msg->source);
        // get current bus status
        bus->get(&bus_status);

        if (mode == DEFAULT){
            dram_access_time = defaultBehavior(); // CL+RCD+RP
        } else {
            dram_access_time = fastBehavior(); // it depends
        }
        // set memory as beep->source
        message *beep = new message();
        strcpy(beep->source, msg->dest);
        beep->timestamp = dram_access_time;
        notify(beep);
    }
}
