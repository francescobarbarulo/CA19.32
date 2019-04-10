#include <iostream>
#include <cstring>
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

enum ModeType {
    DEFAULT, FAST
};

class Memory : public module {
    private:
        // simulation params
        ModeType mode;
        // dram
        uint32_t *dram;
        bool first_read;
        uint16_t last_row_selected;
        // bus interaction
        Bus *bus;
        bus_status bus_status;
    public:
        Memory(string, int priority = 0);
        Memory(string, int, ModeType, Bus*);
        void onNotify(message*);
        bool isSelfMessage(message*);
};

/* TODO */
/* timer ram refresh
*/
