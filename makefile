MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

CPP=clang++
CXX=$(shell root-config --cxx) -I. -O2 -std=c++11  
DEBUGFLAG= -g
CPPFLAGS= -std=c++17 -I=SIM/** -Wall -pedantic -pthread -O3
GUICPPFLAGS= -std=c++17 -I=GUI/** 
LIBS= -L SIM/

DOBJS=SIM/simulation/simulation.o SIM/particles/electron.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/basic/vector.o SIM/mt/multithreading.o SIM/gnuplot/sim_renderer.o SIM/uihandler/ui_handler.o
DOBJS_DBG=SIM/simulation/simulation_dbg.o SIM/particles/electron_dbg.o SIM/particles/bunch_dbg.o SIM/particles/gun_dbg.o SIM/fields/fields_dbg.o SIM/basic/vector_dbg.o SIM/mt/multithreading_dbg.o SIM/gnuplot/sim_renderer_dbg.o SIM/uihandler/ui_handler_dbg.o

ROOTFLAGS=$(shell root-config --glibs --cflags --libs)

GUI_CONFIG_FRAME_DOBJS=GUI/frames/config_frame/gui_config_frame_g.o
GUI_DOBJS=GUI/comm/sim_server_g.o $(GUI_CONFIG_FRAME_DOBJS) GUI/frames/render_frame/gui_render_frame_g.o GUI/frames/ribbon_frame/gui_main_buttons_g.o GUI/frames/gui_main_frame_g.o GUI/renderer/gui_renderer_g.o

GUI_DOBJS_1=GUI/analyzer/gui_analyzer_g.o GUI/comm/sim_server_g.o GUI/data/gui_sim_dataprovider_g.o GUI/frames/gui_main_frame_g.o
GUI_DOBJS_2=GUI/frames/analysis_frame/gui_analysis_frame_g.o GUI/frames/analysis_frame/analysis_control_frames/gui_E_dist_analysis_frame_g.o GUI/frames/analysis_frame/analysis_control_frames/gui_t_vs_E_analysis_frame_g.o
GUI_DOBJS_3=GUI/frames/config_frame/gui_config_frame_g.o GUI/frames/config_frame/B_config_frame/gui_B_config_frame_g.o GUI/frames/config_frame/E_config_frame/gui_E_config_frame_g.o
GUI_DOBJS_4=GUI/frames/config_frame/gun_config_frame/gui_gun_config_frame_g.o GUI/frames/config_frame/sim_config_frame/gui_sim_config_frame_g.o
GUI_DOBJS_5=GUI/frames/render_frame/gui_render_frame_g.o GUI/frames/ribbon_frame/gui_main_buttons_g.o GUI/frames/run_frame/gui_run_frame_g.o
GUI_DOBJS_6=GUI/frames/sweep_frame/gui_sweep_frame_g.o GUI/frames/sweep_frame/sweep_control_frames/gui_sweep_base_subframe_g.o GUI/frames/sweep_frame/sweep_control_frames/gui_sweep_phaselag_frame_g.o
GUI_DOBJS_7=GUI/msgbox/gui_msgbox_handler_g.o GUI/renderer/gui_renderer_g.o

GUI_SIM_DEP=SIM/basic/vector.o

GUI_DOBJS_NEW= $(GUI_DOBJS_1) $(GUI_DOBJS_2) $(GUI_DOBJS_3) $(GUI_DOBJS_4) $(GUI_DOBJS_5) $(GUI_DOBJS_6) $(GUI_DOBJS_7) $(GUI_SIM_DEP)

FW_DOBJS=SIM/basic/vector.o SIM/fields/fields.o SIM/particles/electron.o
	


RUST=$(shell cargo build --release)
CDFR=$(shell cd ./FieldReducer)
CDBACK=$(shell cd ..)


clean_o=rm -rf *.o SIM/**/*.o GUI/**/*.o


.PHONY: fieldreducer

simrhodo.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodo_debug.exe: SIM/main_new.cpp $(DOBJS_DBG)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS_DBG) -o $@ $(LIBS) 
	mkdir -p xy/paths
	$(clean_o)

simrhodoGUI.exe: GUI/gui_refactored.cpp $(GUI_DOBJS_NEW)
	$(CXX) $(GUICPPFLAGS) GUI/gui_refactored.cpp $(GUI_DOBJS_NEW) -o $@ $(ROOTFLAGS) 
	$(clean_o)

simrhodoGUI_debug.exe: GUI/gui_refactored.cpp $(GUI_DOBJS_NEW)
	$(CPP) $(DEBUGFLAG) $(GUICPPFLAGS) GUI/gui_refactored.cpp $(GUI_DOBJS_NEW) -o $@ $(ROOTFLAGS)
	$(clean_o)

simrhodoGUI_ez.exe: GUI/gui_refactored.cpp
	$(CPP) $(GUICPPFLAGS) GUI/gui_refactored.cpp -o $@ $(ROOTFLAGS)
	$(clean_o)

fieldreducer.exe: 	
	$(RUST) \
	$(mv ./target/release/fieldreducer.exe ../fieldreducer.exe) \

fieldview.exe: FieldView/fieldview.cpp $(DOBJS_DBG)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) FieldView/fieldview.cpp $(DOBJS_DBG) -o $@ $(LIBS) 
	$(clean_o)

%.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) 

%_dbg.o: %.cpp
	$(CPP) $(DEBUGFLAG) -c $< -o $@ $(CPPFLAGS) 

%_g.o: %.cpp
	$(CXX) -c $< -o $@ $(GUICPPFLAGS) -I `root-config --incdir` 

release: simrhodo.exe

debug: simrhodo_debug.exe

all: release debug

clean: 
	rm -rf *.o SIM/**/*.o *.exe GUI/**/*.o GUI/*.o GUI/**/**/*.o GUI/**/**/**/*.o

cleanlogs:
	rm -rf xy/**

gui: simrhodoGUI.exe

gui_debug: simrhodoGUI_debug.exe

gui_ez: simrhodoGUI_ez.exe

fieldreducer: 
	cd fieldreducer && cargo build --release && mv ./target/release/fieldreducer ../fieldreducer.exe && rm -r target && cd ..

fieldview: fieldview.exe