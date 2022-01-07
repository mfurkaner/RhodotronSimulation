#ifndef SIMULATION_H
    #include "simulation.h"
#endif
#ifndef VECTOR_H
    #include "vector.h"
#endif


void Simulator::run(DataStorage& path, DataStorage& rf){
    if(multi_threading){
        cout << thread_count << endl;
        bunch.divide(thread_count);
    }
    while ( simulation_time < end_time ){
        E_field.update(simulation_time);
        if ( total_steps%100 == 0 ){
            E_field.log(rf, simulation_time);
            path << "t: " << simulation_time << "   ";
            path << setprecision(4) << "v: " << bunch.e[99].vel << "    E: " << setprecision(6) << bunch.e[99].Et - E0 << "   pos: " << bunch.e[99].pos <<"\n";
        }
        if( multi_threading ){
            MTEngine.doWork(bunch.subBunchPtr(), E_field, B_field, simulation_time, time_interval, rf);
            MTEngine.join();
        }
        else{
            bunch.interact(E_field, B_field, simulation_time, time_interval , path);
        }
        simulation_time += time_interval;
        total_steps++;
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