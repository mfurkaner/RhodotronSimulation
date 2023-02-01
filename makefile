MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
DEBUGFLAG= -g
CPPFLAGS= -std=c++17 -I=SIM/** -Wall -pedantic -pthread -O3
LIBS= -L SIM/

DOBJS=SIM/simulation/simulation.o SIM/particles/electron.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/basic/vector.o SIM/mt/multithreading.o SIM/gnuplot/sim_renderer.o SIM/uihandler/ui_handler.o
DOBJS_DBG=SIM/simulation/simulation_dbg.o SIM/particles/electron_dbg.o SIM/particles/bunch_dbg.o SIM/particles/gun_dbg.o SIM/fields/fields_dbg.o SIM/basic/vector_dbg.o SIM/mt/multithreading_dbg.o SIM/gnuplot/sim_renderer_dbg.o SIM/uihandler/ui_handler_dbg.o

ROOTFLAGS=$(shell root-config --glibs --cflags)

GUI_CONFIG_FRAME_DOBJS=GUI/frames/config_frame/gui_config_frame_g.o
GUI_DOBJS=GUI/comm/sim_server_g.o $(GUI_CONFIG_FRAME_DOBJS) GUI/frames/render_frame/gui_render_frame_g.o GUI/frames/ribbon_frame/gui_main_buttons_g.o GUI/frames/gui_main_frame_g.o GUI/renderer/gui_renderer_g.o

clean_o=rm -rf *.o SIM/**/*.o GUI/**/*.o

simrhodo.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodo_debug.exe: SIM/main_new.cpp $(DOBJS_DBG)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS_DBG) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodoGUI.exe: GUI/gui_refactored.cpp $(GUI_DOBJS)
	$(CPP) $(ROOTFLAGS) GUI/gui_refactored.cpp $(GUI_DOBJS) -o $@

%.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) 

%_dbg.o: %.cpp
	$(CPP) $(DEBUGFLAG) -c $< -o $@ $(CPPFLAGS) 

%_g.o: %.cpp
	$(CPP) $(ROOTFLAGS) -c $< -o $@ $(CPPFLAGS) 

release: simrhodo.exe

debug: simrhodo_debug.exe

all: release debug

clean: 
	rm -rf *.o SIM/**/*.o *.exe GUI/**/*.o GUI/*.o GUI/**/**/*.o

cleanlogs:
	rm -rf xy/**

gui: simrhodoGUI.exe