#include "ExampleModule.h"
#include "../memory/memory.h"
#include "System.h"

using namespace std;

int main() {
	System system;

	//Initialize the system here by adding modules to the system object
	system.addModule(new ExampleModule("Alice"));
	system.addModule(new ExampleModule("Bob", 10));//The second parameter is the priority. It's useful only when you need to intercept someone's else message
	system.addModule(new Memory("MEM"));
	//Call run() to start the simulation
	system.run();
}
