MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
DEBUGFLAG= -g
CPPFLAGS= -std=c++11 -I=SIM/** 
LIBS= -L SIM/


DOBJS=SIM/simulation/simulation.o SIM/particles/electron.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/basic/vector.o SIM/mt/multithreading.o

clean_o=rm -rf *.o SIM/**/*.o

simrhodo.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodo_debug.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

$(ROBJS): %.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) 

$(ROBJSDBG): %.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) $(DEBUGFLAG)

release: simrhodo.exe

debug: simrhodo_debug.exe

clean: 
	rm -rf *.o SIM/**/*.o *.exe
