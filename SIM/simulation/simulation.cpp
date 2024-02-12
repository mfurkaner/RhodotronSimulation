#include "simulation.h"
#include "../basic/vector.h"
#include <chrono>


void Simulator::run(){

    // Before starting, log the magnetic field to the mag file
    cout << "you are in simulator::run " << endl;   

}

void Simulator::saveElectronsInfo(double time){
    gun.saveInfo(time);
}

void Simulator::logPaths(){
    gun.logPaths(pathsStorage, pathsPath, eLogHeader);
}

/*
Electron& Simulator::getElectronWithMaxEnergy(){
    return bunch.getFastest();
}*/

void RhodotronSimulator::addMagnet(double B, double r, vector3d position){
    B_field.addMagnet(B, r, position);
}

void RhodotronSimulator::addMagnet(Magnet m){
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
    if (config.isEinStdSet()) setEinStd(config.getEinStd());
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

void RhodotronSimulator::run(){ 
    // B field is static -> log before starting the simulation
    state_mutex->lock();
    state |= SIM_RUNNING;
    state_mutex->unlock();
    logBfield();     

    if (MULTI_THREAD)
        _runMT();
    else
        _runST();

}

void RhodotronSimulator::stop(){
    closeLogs();
    
    if(MULTI_THREAD){
        MTEngine.stopPool();
    }
    logPaths();
}

void RhodotronSimulator::_runMT(){

    gun.fireAllWithFireTimesMT();

    MTEngine.setupPool(time_interval, start_time, end_time, gun, E_field, B_field, gun.thread_bunchs);
 
    STEPS_TAKEN = 0;
    simulation_time = start_time;
    while (simulation_time < end_time + time_interval ){
        if (STEPS_TAKEN % log_interval() == 0){
            E_field.update(simulation_time);
            logEfield(simulation_time, simulation_time + time_interval > end_time);
            notifyUI(MTEngine.getAverageTime());
        }
        simulation_time+=time_interval;
        STEPS_TAKEN++;
    }
    bool end = false;
    while (!end){
        double time = MTEngine.getAverageTime();
        notifyUI(time);
        if ( time >= end_time ){
            end = true;
        }
        this_thread::yield();
    }
    MTEngine.join();
    
}

void RhodotronSimulator::_runST(){
    while ( simulation_time < end_time + time_interval ){   
        E_field.update(simulation_time);
        
        if ( STEPS_TAKEN%log_interval() == 0 ){
            // every 100th step, log the E field
            logEfield(simulation_time, simulation_time + time_interval > end_time );
            saveElectronsInfo(simulation_time);
        }
        // Do the actual work
        // This part is accounts for ~97% of the execution time
        gun.fireIfActive(simulation_time);
        gun.interact(E_field, B_field, simulation_time, time_interval);
        // Work is finished, update UI handlers watchpoint
        simulation_time += time_interval;
        notifyUI(simulation_time);
        STEPS_TAKEN++;
    }
}

 void RhodotronSimulator::notifyUI(double time){
    ui_mutex->lock();
    dummy_time = time;
    ui_mutex->unlock();
 }