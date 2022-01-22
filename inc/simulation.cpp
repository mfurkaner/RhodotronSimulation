#ifndef SIMULATION_H
    #include "simulation.h"
#endif
#ifndef VECTOR_H
    #include "vector.h"
#endif


void Simulator::run(){
    // Before starting, log the magnetic field to the mag file
    logBfield();                                          
    if(MULTI_THREAD){
        cout << MAX_THREAD_COUNT << endl;
        bunch.divide(MAX_THREAD_COUNT);
    }
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( STEPS_TAKEN%log_interval() == 0 ){
            logEfield(simulation_time);
            // every 100th step, log the E field
            saveElectronsInfo(simulation_time);
        }
        if( MULTI_THREAD ){
            MTEngine.doWork(bunch.subBunchPtr(), E_field, B_field, simulation_time, time_interval);
            MTEngine.join();
        }
        else{
            bunch.interact(E_field, B_field, simulation_time, time_interval);
        }
        simulation_time += time_interval;
        STEPS_TAKEN++;
    }
}

void Simulator::saveElectronsInfo(double time){
    bunch.saveInfo(time);
}

void Simulator::logPaths(){
    system("rm xy/paths/**");
    for(int i = 0; i < NUM_OF_ELECTRONS ; i++){
        pathsStorage.at(i).open();
        bunch.e[i].loge(pathsStorage[i]);
        pathsStorage.at(i).close();
    }
}

double Simulator::getAverageEnergy(){
    return bunch.E_ave();
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
    //if (config.isPPathSet()) setPath(config.getPpath());
    if (config.isCPathSet()) setConfigPath(config.getCpath());
    if (config.isSTimeSet()) setStartTime(config.getSTime());
    if (config.isETimeSet()) setEndTime(config.getETime());
    if (config.isdTSet()) setdT(config.getdT());
    if (config.isGunTimeSet()) setGunActiveTime(config.getGunTime());
    if (config.isNumOfESet()) setNumberofElectrons(config.getNumOfE());
    if (config.isMTSet()) enableMultiThreading(config.getThCount());
    if (config.isR1Set()) setR1(config.getR1());
    if (config.isR2Set()) setR2(config.getR2());
    if (config.isEinSet()) setEin(config.getEin());
    if (config.areThereMagnets()){
        for(int i = 0; i < config.magnets.size(); i++){
            addMagnet(config.magnets.at(i));
        }
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
    
    setNumberofElectrons(NUM_OF_ELECTRONS);
    bunch.setEin(Ein);
    bunch.setNSLen(GUN_ACTIVE_TIME);
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

extern pthread_mutex_t lck;

void RhodotronSimulator::run(){
    // Before starting, log the magnetic field to the mag file
    logBfield();                                          
    if(MULTI_THREAD){
        cout << MAX_THREAD_COUNT << endl;
        bunch.divide(MAX_THREAD_COUNT);
    }
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( STEPS_TAKEN%log_interval() == 0 ){
            logEfield(simulation_time);
            // every 100th step, log the E field
            saveElectronsInfo(simulation_time);
        }
        if( MULTI_THREAD ){
            MTEngine.doWork(bunch.subBunchPtr(), E_field, B_field, simulation_time, time_interval);
            MTEngine.join();
        }
        else{
            bunch.interact(E_field, B_field, simulation_time, time_interval);
        }
        simulation_time += time_interval;
        pthread_mutex_lock(&lck);
        dummy_time = simulation_time;
        pthread_mutex_unlock(&lck);
        STEPS_TAKEN++;
    }
}