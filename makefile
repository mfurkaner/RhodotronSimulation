MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
DEBUGFLAG= -g -pthread
CPPFLAGS= -std=c++17 -I=SIM/** -Wall -pedantic
LIBS= -L SIM/


DOBJS=SIM/simulation/simulation.o SIM/particles/electron.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/basic/vector.o SIM/mt/multithreading.o
DOBJS_DBG=SIM/simulation/simulation_dbg.o SIM/particles/electron_dbg.o SIM/particles/bunch_dbg.o SIM/particles/gun_dbg.o SIM/fields/fields_dbg.o SIM/basic/vector_dbg.o SIM/mt/multithreading_dbg.o

clean_o=rm -rf *.o SIM/**/*.o

simrhodo.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodo_debug.exe: SIM/main_new.cpp $(DOBJS_DBG)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS_DBG) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

%.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) 

%_dbg.o: %.cpp
	$(CPP) $(DEBUGFLAG) -c $< -o $@ $(CPPFLAGS) 

release: simrhodo.exe

debug: simrhodo_debug.exe

all: release debug

clean: 
	rm -rf *.o SIM/**/*.o *.exe
