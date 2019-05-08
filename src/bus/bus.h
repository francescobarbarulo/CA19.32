#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace std;

enum request_t {READ, WRITE, UNDEFINED};

struct Bus_status {
    request_t request;
    uint16_t address;
    uint32_t data;
};

class Bus {
    private:
        // status
        bool busy;
        request_t request;
        uint16_t address;
        uint32_t data;
    public:
        Bus();
        bool set(Bus_status*);
        bool get(Bus_status*);
        friend ostream& operator<<(ostream&, const Bus&);
};

#endif
