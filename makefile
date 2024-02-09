MACHINE = $(shell uname -s)
LINUX  = Linux
MAC    = Darwin

MSG_BUILDING_SIM_LIBS=$(info        ********** Building Rhodotron Simulation Engine Libraries **********)
MSG_BUILDING_SIM=$(info       ********** Building Rhodotron Simulation Engine **********)
MSG_BUILDING_SIM_DBG=$(info        ********** Building Rhodotron Simulation Engine Debug Version **********)
MSG_BUILDING_GUI_LIBS=$(info       **********Building Rhodotron Simulation GUI Libraries **********)
MSG_BUILDING_GUI=$(info       ********** Building Rhodotron Simulation GUI **********)
MSG_BUILDING_FR=$(info        ********** Building FieldReducer **********)

CPP=clang++
CXX=$(shell root-config --cxx) -I. -O2 -std=c++17  $(shell `root-config --incdir`)
DEBUGFLAG= -g
CPPFLAGS= -std=c++17 -I=SIM/** -Wall -pedantic -pthread -O3 
CPPSHAREDFLAGS= -std=c++17 -I=SIM/** -Wall -pedantic -O3 

LIBS= -L lib -lRhodoVector -lRhodoSimCore -lRhodoSim -lRhodoConfig -lRhodoDataStorage
SHARED_LIBS= -lRhodoRender -lRhodoSimUIhandler 

GUICPPFLAGS=-I=GUI/**
GUI_LIBS= -L lib -lRhodoVector -lRhodoGUIframes -lRhodoGUIhelpers -lRhodoGUIdataprovider

DOBJS=SIM/simulation/simulation.o SIM/particles/particle.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/interact/interactor.o SIM/basic/vector.o SIM/config/configuration.o SIM/mt/multithreading.o SIM/gnuplot/sim_renderer.o SIM/uihandler/ui_handler.o SIM/basic/datastorage.o 
DOBJS_DBG=SIM/simulation/simulation_dbg.o SIM/particles/particle_dbg.o SIM/particles/bunch_dbg.o SIM/particles/gun_dbg.o SIM/fields/fields_dbg.o  SIM/interact/interactor_dbg.o SIM/basic/vector_dbg.o  SIM/config/configuration_dbg.o SIM/mt/multithreading_dbg.o SIM/gnuplot/sim_renderer_dbg.o SIM/uihandler/ui_handler_dbg.o SIM/basic/datastorage_dbg.o 

SHARED_OBJS=SIM/config/configuration.o SIM/gnuplot/sim_renderer.o SIM/uihandler/ui_handler.o SIM/basic/datastorage.o 
SHARED_OBJS_GUI=GUI/data/gui_sim_dataprovider.o

STATIC_OBJS=SIM/basic/vector.o SIM/particles/particle.o SIM/particles/bunch.o SIM/particles/gun.o SIM/fields/fields.o SIM/interact/interactor.o SIM/simulation/simulation.o SIM/mt/multithreading.o SIM/basic/datastorage.o 

ROOTFLAGS=$(shell root-config --glibs --cflags --libs)

GUI_CONFIG_FRAME_DOBJS=GUI/frames/config_frame/gui_config_frame_g.o
GUI_DOBJS=GUI/comm/sim_server_g.o $(GUI_CONFIG_FRAME_DOBJS) GUI/frames/render_frame/gui_render_frame_g.o GUI/frames/ribbon_frame/gui_main_buttons_g.o GUI/frames/MainFrame.o GUI/renderer/gui_renderer_g.o

GUI_DOBJS_1=GUI/analyzer/gui_analyzer_g.o GUI/comm/sim_server_g.o GUI/data/gui_sim_dataprovider_g.o 
GUI_DOBJS_2=GUI/frames/MainFrame_g.o GUI/frames/analysis_frame/gui_analysis_frame_g.o GUI/frames/analysis_frame/analysis_control_frames/gui_E_dist_analysis_frame_g.o GUI/frames/analysis_frame/analysis_control_frames/gui_t_vs_E_analysis_frame_g.o
GUI_DOBJS_3=GUI/frames/config_frame/gui_config_frame_g.o GUI/frames/config_frame/B_config_frame/gui_B_config_frame_g.o GUI/frames/config_frame/E_config_frame/gui_E_config_frame_g.o
GUI_DOBJS_4=GUI/frames/config_frame/gun_config_frame/gui_gun_config_frame_g.o GUI/frames/config_frame/sim_config_frame/gui_sim_config_frame_g.o
GUI_DOBJS_5=GUI/frames/render_frame/gui_render_frame_g.o GUI/frames/ribbon_frame/gui_main_buttons_g.o GUI/frames/run_frame/gui_run_frame_g.o
GUI_DOBJS_6=GUI/frames/sweep_frame/gui_sweep_frame_g.o GUI/frames/sweep_frame/sweep_control_frames/gui_sweep_base_subframe_g.o GUI/frames/sweep_frame/sweep_control_frames/gui_sweep_phaselag_frame_g.o
GUI_DOBJS_7=GUI/msgbox/gui_msgbox_handler_g.o GUI/renderer/gui_renderer_g.o

GUI_DOBJS_NEW= $(GUI_DOBJS_1) $(GUI_DOBJS_2) $(GUI_DOBJS_3) $(GUI_DOBJS_4) $(GUI_DOBJS_5) $(GUI_DOBJS_6) $(GUI_DOBJS_7)
GUI_DOBJS_FRAMES=$(GUI_DOBJS_2) $(GUI_DOBJS_3) $(GUI_DOBJS_4) $(GUI_DOBJS_5) $(GUI_DOBJS_6)

FW_DOBJS=SIM/basic/vector.o SIM/fields/fields.o SIM/particles/particle.o


RUST=$(shell cargo build --release)
CDFR=$(shell cd ./FieldReducer)
CDBACK=$(shell cd ..)


clean_o=rm -rf *.o SIM/**/*.o GUI/**/*.o

%.o : %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS) 

%_dbg.o: %.cpp
	$(CPP) $(DEBUGFLAG) -c $< -o $@ $(CPPFLAGS) 

%_g.o: %.cpp
	$(CXX) -c $< -o $@ $(GUICPPFLAGS) -I `root-config --incdir` 


.PHONY: fieldreducer

simrhodo.exe: 
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp -o $@ $(LIBS) $(SHARED_LIBS) -Wl,-rpath,..
	mkdir -p bin/xy/paths
	mv simrhodo.exe bin/simrhodo.exe
	$(clean_o)

simrhodo_standalone.exe: SIM/main_new.cpp $(DOBJS)
	$(CPP) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS) -o $@
	mkdir -p bin/xy/paths
	mv simrhodo_standalone.exe bin/simrhodo_standalone.exe
	$(clean_o)

simrhodo_debug.exe: SIM/main_new.cpp $(DOBJS_DBG)
	$(CPP) $(DEBUGFLAG) $(CPPFLAGS) SIM/main_new.cpp $(DOBJS_DBG) -o $@ 
	mkdir -p xy/paths
	mv simrhodo_debug.exe bin/simrhodo_debug.exe
	$(clean_o)

simrhodoGUI.exe: 
	rootcint -f GUI/guistream.cxx -c GUI/frames/MainFrame.h
	$(CXX) $(GUICPPFLAGS) -c GUI/guistream.cxx -o GUI/guistream.o -O -fPIC -I `root-config --incdir`
	$(CXX) $(GUICPPFLAGS) GUI/gui_refactored.cpp GUI/guistream.o -o bin/$@ $(ROOTFLAGS) $(GUI_LIBS) -Wl,-rpath,..
	$(clean_o)

simrhodoGUI_standalone.exe: GUI/gui_refactored.cpp $(GUI_DOBJS_NEW)
	rootcint -f GUI/guistream.cxx -c GUI/frames/MainFrame.h
	$(CPP) $(CPPFLAGS) -c SIM/basic/vector.cpp -o SIM/basic/vector.o
	$(CXX) $(GUICPPFLAGS) -c GUI/guistream.cxx -o GUI/guistream.o -O -fPIC -I `root-config --incdir`
	$(CXX) $(GUICPPFLAGS) GUI/gui_refactored.cpp $(GUI_DOBJS_NEW) GUI/guistream.o -o bin/$@ $(ROOTFLAGS)
	$(clean_o)

simrhodoGUI_debug.exe: GUI/gui_refactored.cpp $(GUI_DOBJS_NEW)
	$(CPP) $(DEBUGFLAG) $(GUICPPFLAGS) GUI/gui_refactored.cpp $(GUI_DOBJS_NEW) -o $@ $(ROOTFLAGS)
	$(clean_o)

fieldview.exe: FieldView/fieldview.cpp $(DOBJS_DBG)
	$(CXX) $(DEBUGFLAG) $(CPPFLAGS) FieldView/fieldview.cpp $(DOBJS_DBG) -o $@ $(LIBS) 
	mv fieldview.exe bin/fieldview.exe
	$(clean_o)

release: libs cp_shared simrhodo.exe 

standalone_engine: simrhodo_standalone.exe

debug: simrhodo_debug.exe

e_mbsl:
	$(MSG_BUILDING_SIM_LIBS)
e_mbgl:
	$(MSG_BUILDING_GUI_LIBS)
e_mbs:
	$(MSG_BUILDING_SIM)
e_mbs_d:
	$(MSG_BUILDING_SIM_DBG)
e_mbg:
	$(MSG_BUILDING_GUI)
e_mbfr:
	$(MSG_BUILDING_FR)

all: e_mbsl libs e_mbgl guilibs cp_shared e_mbs release e_mbs_d debug e_mbg gui e_mbfr fieldreducer

clean: 
	rm -rf lib/** bin/lib/** *.o SIM/**/*.o *.exe GUI/*.o GUI/**/*.o GUI/*.o GUI/**/**/*.o GUI/**/**/**/*.o bin/*.exe

libs: $(STATIC_OBJS) $(SHARED_OBJS) 
	ar cr lib/libRhodoVector.a SIM/basic/vector.o
	ar cr lib/libRhodoDataStorage.a SIM/basic/datastorage.o
	ar cr lib/libRhodoConfig.a SIM/config/configuration.o SIM/basic/datastorage.o
	ar cr lib/libRhodoSimCore.a SIM/fields/fields.o SIM/particles/particle.o SIM/particles/bunch.o SIM/particles/gun.o SIM/interact/interactor.o
	ar cr lib/libRhodoSim.a SIM/simulation/simulation.o SIM/mt/multithreading.o
	$(CPP) -shared $(CPPSHAREDFLAGS) -o lib/libRhodoRender.so SIM/gnuplot/sim_renderer.o lib/libRhodoDataStorage.a lib/libRhodoConfig.a lib/libRhodoVector.a 
	$(CPP) -shared $(CPPSHAREDFLAGS) -o lib/libRhodoSimUIhandler.so SIM/uihandler/ui_handler.o

guilibs: $(GUI_DOBJS_NEW) $(SHARED_OBJS_GUI)
	ar cr lib/libRhodoGUIhelpers.a GUI/analyzer/gui_analyzer_g.o GUI/comm/sim_server_g.o GUI/msgbox/gui_msgbox_handler_g.o GUI/renderer/gui_renderer_g.o
	ar cr lib/libRhodoGUIframes.a $(GUI_DOBJS_FRAMES)
	$(CPP) -shared $(CPPSHAREDFLAGS) -o lib/libRhodoGUIdataprovider.so GUI/data/gui_sim_dataprovider.o lib/libRhodoVector.a	

cp_shared:
	cp lib/libRhodoRender.so bin/lib/libRhodoRender.so
	cp lib/libRhodoSimUIhandler.so bin/lib/libRhodoSimUIhandler.so 
	cp lib/libRhodoGUIdataprovider.so bin/lib/libRhodoGUIdataprovider.so

cleanlogs:
	rm -rf xy/**

gui: libs guilibs cp_shared simrhodoGUI.exe

gui_debug: simrhodoGUI_debug.exe

fieldreducer: 
	cd fieldreducer && cargo build --release && mv ./target/release/fieldreducer ../bin/fieldreducer.exe && rm -r target && cd ..

fieldview: fieldview.exe