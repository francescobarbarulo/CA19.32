#include "bus.h"

Bus::Bus(){
    request = UNDEFINED;
    busy = false;
}

/*
Bus* Bus::getInstance(){
    if (!instance){
        instance = new Bus();
    }
    return instance;
}
*/

bool Bus::set(Bus_status *s){
    if (busy){
        // It is not possible to change bus status while components are accessing to it
        cout << "[BUS] trying to set before getting the status" << endl;
        return false;
    }

    request = s->request;
    address = s->address;
    data = s->data;
    busy = true;

    return true;
}

bool Bus::get(Bus_status *s){
    if (!busy){
        // the bus status is meaningless
        cout << "[BUS] trying to get before setting the status" << endl;
        return false;
    }

    s->request = request;
    s->address = address;
    s->data = data;
    busy = false;

    return true;
}

ostream& operator<<(ostream& os, const Bus& b){
    if (b.request == READ)
        os << "/mr\t0\n/mw\t1\n";
    else if (b.request == WRITE)
        os << "/mr\t1\n/mw\t0\n";
    else
        os << "/mr\t1\n/mw\t1\n";
    os << "a15_a0\t0x" << setfill ('0') << setw(4) << hex << b.address << endl;
    os << "d31_d0\t0x" << setfill ('0') << setw(8) << hex << b.data << endl;

    if (b.busy) os << "BUSY";
    else os << "IDLE";
    os << endl;

    return os;
}
