#include <cstdint>
#include <iostream>

enum request_t {READ, WRITE};

struct bus_status {
    request_t request;
    uint16_t address;
    uint32_t data;
};

class Bus {
    private:
        // singleton pattern
        //static Bus *instance;
        // status
        bool busy;
        /* ( It is possible to merge _mr and _mw in enum request_t */
        bool _mr;
        bool _mw;
        /* ) */
        uint16_t address;
        uint32_t data;
    public:
        Bus();
        //static Bus *getInstance();
        bool set(bus_status *s); // return true if the bus is idle
        bool get(bus_status *s);
        void print_status();
};
