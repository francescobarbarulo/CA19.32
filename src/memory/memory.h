#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include "../orchestrator/module.h"
#include "../orchestrator/structures.h"
#include "../bus/bus.h"

using namespace std;

// dram size definition (64k)
#define DRAM_SIZE 1 << 14

// timing definition
#define CL  7
#define RCD 8
#define RP  8
#define RAS 24

#define REFRESH_INTERVAL 20

enum ModeType {
    DEFAULT, FAST
};

// sim params definition
#define MODE FAST

class Memory : public module {
    using module::module;
    private:
        // simulation params
        ModeType mode = MODE;
        // dram
        uint32_t *dram;
        bool first_access;
        uint16_t last_row_addressed;
        bool refreshing_phase;
        // bus interaction
        Bus *bus;
        Bus_status bus_status;
    public:
        Memory(string, int, Bus*);
        void onNotify(message*);
        message* createMessage(string);
        bool isSelfMessage(message*);
        int defaultBehavior();
        int optimizedBehavior(uint16_t);
        // refreshin phase
        void startRefresh();
        void endRefresh();
};

#endif
