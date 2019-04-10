CXX=g++
CPPFLAGS=-std=c++11

# Header files
CXX_MEMORY_HDR=$(wildcard memory/*.h)
CXX_BUS_HDR=$(wildcard bus/*.h)
CXX_ORCHESTRATOR_HDR=$(wildcard orchestrator/*.h)

CXX_MEMORY_SRC=$(wildcard memory/*.cpp)
CXX_MEMORY_OBJ=$(patsubst memory/%.cpp, obj/memory_%.o, $(CXX_MEMORY_SRC))

CXX_BUS_SRC=$(wildcard bus/*.cpp)
CXX_BUS_OBJ=$(patsubst bus/%.cpp, obj/bus_%.o, $(CXX_BUS_SRC))

CXX_ORCHESTRATOR_SRC=$(wildcard orchestrator/*.cpp)
CXX_ORCHESTRATOR_OBJ=$(patsubst orchestrator/%.cpp, obj/orchestrator_%.o, $(CXX_ORCHESTRATOR_SRC))

sim: environ $(CXX_ORCHESTRATOR_OBJ) $(CXX_BUS_OBJ) $(CXX_MEMORY_OBJ)
	$(CXX) $(CPPFLAGS) -o sim obj/*.o

environ:
	mkdir -p obj

obj/memory_%.o: memory/%.cpp $(CXX_MEMORY_HDR)
	$(CXX) -c $(CPPFLAGS) -o $@ $<

obj/bus_%.o: bus/%.cpp $(CXX_BUS_HDR)
	$(CXX) -c $(CPPFLAGS) -o $@ $<

obj/orchestrator_%.o: orchestrator/%.cpp $(CXX_ORCHESTRATOR_HDR)
	$(CXX) -c $(CPPFLAGS) -o $@ $<



clean:
	rm -rf obj sim
