#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <fstream>
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

#define MS_INTERVAL         32 // [ms]
#define FREQUENCY           32 // [KHz]
#define REFRESHING_INTERVAL MS_INTERVAL * FREQUENCY
#define REFRESHING_TIME     RCD * (1 << 6) // 2^6 rows

enum ModeType {
    DEFAULT, FAST
};

// sim params definition
#define MODE_TYPE FAST

class Memory : public module {
    using module::module;
    private:
        // dram
        uint32_t *dram;
        bool first_access;
        uint16_t last_row_addressed;
        // refreshing phase
        bool refreshing_phase;
        unsigned int refreshing_phase_started_at;
        // bus interaction
        Bus *bus;
        Bus_status bus_status;
    public:
        Memory(string, int, Bus*, string prog = "");
        void onNotify(message*);
        message* createMessage(string, string);
        bool isSelfMessage(message*);
        int defaultBehavior();
        int optimizedBehavior(uint16_t);
        // refreshing phase
        void startRefresh();
        void endRefresh();
};

#endif
