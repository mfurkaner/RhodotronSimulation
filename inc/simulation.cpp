#ifndef SIMULATION_H
    #include "simulation.h"
#endif
#ifndef VECTOR_H
    #include "vector.h"
#endif


void Simulator::run(){
    // Before starting, log the magnetic field to the mag file
    logBfield();                                          
    if(multi_threading){
        cout << thread_count << endl;
        bunch.divide(thread_count);
    }
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( total_steps%log_interval() == 0 ){
            logEfield(simulation_time);
            // every 100th step, log the E field
            saveElectronsInfo(simulation_time);
        }
        if( multi_threading ){
            MTEngine.doWork(bunch.subBunchPtr(), E_field, B_field, simulation_time, time_interval);
            MTEngine.join();
        }
        else{
            bunch.interact(E_field, B_field, simulation_time, time_interval);
        }
        simulation_time += time_interval;
        total_steps++;
    }
    //logPaths();
}

void Simulator::saveElectronsInfo(double time){
    bunch.saveInfo(time);
}

void Simulator::logPaths(){
    system("rm xy/paths/**");
    for(int i = 0; i < num_of_electrons ; i++){
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