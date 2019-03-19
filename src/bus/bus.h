#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace std;

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
        request_t request;
        uint16_t address;
        uint32_t data;
    public:
        Bus();
        //static Bus *getInstance();
        bool set(bus_status*); // return true if the bus is idle
        bool get(bus_status*);
        friend ostream& operator<<(ostream&, const Bus&);
};
