#include "electron.h"



std::ostream& operator<<(std::ostream& stream, ElectronLog& log){
    stream << log.time << "," << log.energy << "," << log.pos << "," << log.vel;
    return stream;
}
 
#pragma region ELECTRON
double Electron2D::get_vel(){
    return c*sqrt(Et*Et-E0*E0)/Et;
}

void Electron2D::print_electron_info(){
    //cout<<std::setprecision(4);
    for(int i = 0; i < t_giris_cikis.size() ; i++ ){
        //cout << "\tGecis " << i+1 << ") " << "Enerji : " << enerjiler.at(i)-E0 << " MeV, giris zamani : " << t_giris_cikis.at(i).first << " ns, cikis zamani : " << t_giris_cikis.at(i).second << " ns" << endl;
    }
    //cout << "Energy : " << Et - E0 << "    Position : " << pos << "    Velocity : " << vel << endl;
}

void Electron2D::move(double dt){
    pos += vel*(dt*ns);
}

void Electron2D::move(const vector3d& acc, double dt){
    pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2;
}

void Electron2D::move(const vector3d& acc, const vector3d& jerk, double dt){
    pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2 + jerk*(dt*ns*dt*ns*dt*ns)/6 ;
}

void Electron2D::accelerate(const vector3d& acc, const vector3d& jerk, double dt){
    vel += acc*(dt*ns) + jerk*(dt*ns*dt*ns)/2;  
    Et = gamma()*E0;                                                
}

void Electron2D::accelerate(const vector3d& acc, double dt){
    vel += acc*(dt*ns);  
    Et = gamma()*E0;                                                
}

void Electron2D::interactLF(RFField& E, MagneticField& B, double time_interval){
    vector3d acc_E = E.actOn(*this);
    vector3d acc_B = B.actOn(*this);

    vector3d acc = acc_E + acc_B;
    move( acc, time_interval/2);
    accelerate( acc, time_interval);
    move( acc, time_interval/2);
}

void Electron2D::interactRK(RFField& E, MagneticField& B, double time_interval){
    vector3d run_kut_E = E.actOnAndGetRungeKuttaCoef(*this, time_interval);
    vector3d run_kut_B = B.actOnAndGetRungeKuttaCoef(*this, time_interval);

    vector3d acc = run_kut_E + run_kut_B;
    
    move(acc, time_interval/2);
    accelerate(acc, time_interval);
    move(acc, time_interval/2);
}
