MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
CPPFLAGS= -std=c++11 -I=inc/ 
LIBS= -L lib/


DOBJS=inc/simulation.o inc/rhodo2d.o inc/fields.o inc/vector.o inc/multithreading.o

simrhodo.exe: main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) main_new.cpp $(DOBJS) -o $@ $(LIBS) 

$(ROBJS): %.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS)

all: simrhodo.exe

clean: 
	rm -rf *.o *.exe
