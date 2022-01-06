#ifndef SIMULATION_H
    #include "simulation.h"
#endif
#ifndef VECTOR_H
    #include "vector.h"
#endif


void Simulator::run(DataStorage& path, DataStorage& rf){
    int step = 0;
    bool draw = false;
    while ( simulation_time < end_time ){
        draw = false;
        E_field.update(simulation_time);
        if ( step%100 == 0 ){
            E_field.log(rf, simulation_time);
            path << "t: " << simulation_time << "   ";
            draw = true;
        }
        bunch.interact(E_field, B_field, simulation_time, time_interval , path, draw);
        simulation_time += time_interval;
        step++;
    }
}

double Simulator::getAverageEnergy(){
    return bunch.E_ave();
}

Electron2D& Simulator::getElectronWithMaxEnergy(){
    return bunch.e[ bunch.index_fastest ];
}

void Simulator::addMagnet(double B, double r, vector3d position){
    B_field.addMagnet(B, r, position);
}

void Simulator::addMagnet(Magnet m){
    B_field.addMagnet(m);
}