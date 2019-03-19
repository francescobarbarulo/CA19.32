#include "bus.h"

Bus::Bus(){
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

bool Bus::set(bus_status *s){
    if (busy){
        // It is not possible to change bus status while components are accessing to it
        return false;
    }

    request = s->request;
    address = s->address;
    data = s->data;
    busy = true;

    return true;
}

bool Bus::get(bus_status *s){
    if (!busy){
        // the bus status is meaningless
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
    else
        os << "/mr\t1\n/mw\t0\n";
    os << "a15_a0\t0x" << setfill ('0') << setw(4) << hex << b.address << endl;
    os << "d31_d0\t0x" << setfill ('0') << setw(8) << hex << b.data << endl;

    return os;
}
