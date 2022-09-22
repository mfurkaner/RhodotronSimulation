MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
CPPFLAGS= -std=c++11 -I=SIM/** 
LIBS= -L SIM/


DOBJS=SIM/simulation/simulation.o SIM/rhodo/rhodo2d.o SIM/fields/fields.o SIM/basic/vector.o SIM/mt/multithreading.o

simrhodo.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 

$(ROBJS): %.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS)

all: simrhodo.exe

clean: 
	rm -rf *.o SIM/**/*.o *.exe
