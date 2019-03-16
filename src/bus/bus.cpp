#include "bus.h"

Bus::Bus(){
    busy = false;
    _mr = 1;
    _mw = 1;
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

    if (s->r == READ){
        _mr = 0;
        _mw = 1;
    } else {
        _mr = 1;
        _mw = 0;
    }
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

    s->r = (_mr == 0 && _mw == 1) ? READ : WRITE;
    s->address = address;
    s->data = data;
    busy = false;

    return true;
}

void Bus::print_status(){
    printf("/mr\t%d\n", (int)_mr);
    printf("/mw\t%d\n", (int)_mw);
    printf("a15_a0\t0x%04x\n", address);
    printf("d31_d0\t0x%08x\n", data);
}
