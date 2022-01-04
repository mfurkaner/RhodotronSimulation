#ifndef SIMULATION_H
    #include "simulation.h"
#endif
#ifndef VECTOR_H
    #include "vector.h"
#endif


void Simulator::run(DataStorage& ds){
    int steps = 0;
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        ds << "Time : " << simulation_time << "       ";
        bunch.interact(E_field, B_field, simulation_time, time_interval , ds);
        simulation_time += time_interval;
        /*
        if ( steps%20 == 0 && simulation_time > 9){
            system( "read -n 1 -s -p \"Press any key to continue...\"" );
            cout << "\n";
        }*/
        steps++;
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