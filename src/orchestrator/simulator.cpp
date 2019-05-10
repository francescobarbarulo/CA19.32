#include "ExampleModule.h"
#include "CPUTest.h"
#include "../memory/memory.h"
#include "System.h"
#include "parameters.h"

using namespace std;

Parameters params;

int main(int argc, char* argv[]) {
	parseCommand(argc, argv);

	System system;
	Bus main_bus = Bus();

	//Initialize the system here by adding modules to the system object

	//system.addModule(new ExampleModule("Alice"));
	//system.addModule(new ExampleModule("Bob", 10));//The second parameter is the priority. It's useful only when you need to intercept someone's else message

	system.addModule(new CPUTest("CPU", 0, &main_bus));
	system.addModule(new Memory("MEM", 0, &main_bus));
	//Call run() to start the simulation
	system.run();
}
