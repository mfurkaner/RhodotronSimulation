#include "simulation.h"
#include "../basic/vector.h"


void Simulator::run(){
    // Before starting, log the magnetic field to the mag file
    cout << "aa you are in simulator::run " << endl;
    logBfield();      
    cout << "you are in simulator::run " << endl;
    /*                                    
    if(MULTI_THREAD){
        //cout << MAX_THREAD_COUNT << endl;
        bunch.divide(MAX_THREAD_COUNT);
    }*/
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( STEPS_TAKEN%log_interval() == 0 ){
            logEfield(simulation_time);
            // every 100th step, log the E field
            saveElectronsInfo(simulation_time);
        }
        if( MULTI_THREAD ){
            /*
            MTEngine.doWork(bunch.subBunchPtr(), E_field, B_field, simulation_time, time_interval);
            MTEngine.join();*/
        }
        else{
            gun.interact(E_field, B_field, simulation_time, time_interval);
        }
        simulation_time += time_interval;
        STEPS_TAKEN++;
    }
}

void Simulator::saveElectronsInfo(double time){
    gun.saveInfo(time);
}

void Simulator::logPaths(){
    gun.logPaths(pathsStorage, pathsPath, eLogHeader);
}

/*
Electron2D& Simulator::getElectronWithMaxEnergy(){
    return bunch.getFastest();
}*/

void Simulator::addMagnet(double B, double r, vector3d position){
    B_field.addMagnet(B, r, position);
}

void Simulator::addMagnet(Magnet m){
    B_field.addMagnet(m);
}



void RhodotronSimulator::getConfig(Configuration& config){

    if (config.isEmaxSet()) setEmax(config.getEmax());
    if (config.isFreqSet()) setFreq(config.getFreq());
    if (config.isPhaseLagSet()) setPhaseLag(config.getPhaseLag());
    if (config.isEPathSet()) setRFPath(config.getEpath());
    if (config.isBPathSet()) setBPath(config.getBpath());
    if (config.isPPathSet()) setPathsPath(config.getPpath());
    if (config.isCPathSet()) setConfigPath(config.getCpath());
    if (config.isSTimeSet()) setStartTime(config.getSTime());
    if (config.isETimeSet()) setEndTime(config.getETime());
    if (config.isdTSet()) setdT(config.getdT());
    if (config.isGunTimeSet()) setGunActiveTime(config.getGunTime());
    if (config.isGunPeriodSet())setGunPeriod(config.getGunPeriod());
    if (config.isNumOfESet()) setNumberofElectrons(config.getNumOfE());
    if (config.isNumOfBunchSet()) setNumberofBunchs(config.getNumOfB());
    if (config.isMTSet()) enableMultiThreading(config.getThCount());
    if (config.isR1Set()) setR1(config.getR1());
    if (config.isR2Set()) setR2(config.getR2());
    if (config.isEinSet()) setEin(config.getEin());
    if (config.areThereMagnets()){
        for(int i = 0; i < config.magnets.size(); i++){
            addMagnet(config.magnets.at(i));
        }
    }
    if (config.debugMode()){
        DEBUG = true;
    }
    
    config.logConfiguration(configPath);
    updateSimulation();
}


void RhodotronSimulator::updateSimulation(){
    E_field = CoaxialRFField(phase_lag);
    E_field.setEmax(Emax);
    E_field.setEmaxPos(R1);
    E_field.setFreq(freq);
    E_field.setR1(R1);
    E_field.setR2(R2);
    E_field.update(0);
    
    gun.setEin(Ein);
    gun.setGunActiveTime(GUN_ACTIVE_TIME);
    gun.setGunInterval(GUN_PERIOD);
    gun.setGunDir(gunDirection);
    gun.setGunPos(gunPosition);
    setNumberofElectrons(NUM_OF_ELECTRONS);
}

// Emax
// freq
// phase_lag
// EfieldStorage
// BfieldStorage
// pathsStorage
// start time
// end time
// time interval
// gun active time
// number of electrons
// multithread
// thread count
// r1
// r2

extern mutex mutex_lock;

void RhodotronSimulator::run(){                                 // TODO : implement Multithreading
    // Before starting, log the magnetic field to the mag file
    logBfield();                           
    if(MULTI_THREAD){
        gun.bunchs[0].divide(MAX_THREAD_COUNT);
    }
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( STEPS_TAKEN%log_interval() == 0 ){
            logEfield(simulation_time);
            // every 100th step, log the E field
            saveElectronsInfo(simulation_time);
        }
        if( MULTI_THREAD ){
            MTEngine.doWork(gun.bunchs[0].subBunchPtr(), E_field, B_field, simulation_time, time_interval);
            MTEngine.join();
        }
        else{
            gun.interact(E_field, B_field, simulation_time, time_interval);
        }
        simulation_time += time_interval;
        mutex_lock.lock();
        dummy_time = simulation_time;
        mutex_lock.unlock();
        STEPS_TAKEN++;
    }
    if(MULTI_THREAD){
        gun.bunchs[0].concat();
    }
}
