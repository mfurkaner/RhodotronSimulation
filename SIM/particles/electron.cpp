#include "electron.h"



std::ostream& operator<<(std::ostream& stream, ElectronLog& log){
    stream << log.time << "," << log.energy << "," << log.pos << "," << log.vel;
    return stream;
}
 
#pragma region ELECTRON
double Electron2D::get_vel(){
    return c*sqrt(Et*Et-E0_sq)/Et;
}

void Electron2D::print_electron_info(){/*
    //cout<<std::setprecision(4);
    for(int i = 0; i < t_giris_cikis.size() ; i++ ){
        //cout << "\tGecis " << i+1 << ") " << "Enerji : " << enerjiler.at(i)-E0 << " MeV, giris zamani : " << t_giris_cikis.at(i).first << " ns, cikis zamani : " << t_giris_cikis.at(i).second << " ns" << endl;
    }*/
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

void Electron2D::interactRK_ActorE(const RFField& E, const MagneticField& B, double time_interval){
    vector3d run_kut_E = interactE_RK(E, time_interval);
    vector3d run_kut_B = interactB_RK(B, time_interval);

    vector3d acc = run_kut_E + run_kut_B;
    
    move(acc, time_interval/2);
    accelerate(acc, time_interval);
    move(acc, time_interval/2);
}

vector3d Electron2D::interactE_RK(const RFField& E, double time_interval){
    Electron2D e_dummy;
    e_dummy.Et = Et;
    e_dummy.pos = pos;
    e_dummy.vel = vel;
    double dt_halved = time_interval/2;
    // get k1                                    // Calculate E vector
    vector3d F_m = E.getField(e_dummy.pos)*1E6*eQMratio;                                             // Calculate F/m vector
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k2
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k1, dt_halved);
    F_m = E.getField(e_dummy.pos)*1E6*eQMratio;                                                      // Calculate F/m vector
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k3
    e_dummy.vel = vel;
    e_dummy.accelerate(k2, dt_halved);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k4
    e_dummy.vel = vel;
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k3, time_interval);
    F_m = E.getField(e_dummy.pos)*1E6*eQMratio;   
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

    return (k1 + k2*2 + k3*2 + k4)/6;
}

vector3d Electron2D::interactB_RK(const MagneticField& B, double time_interval){
    if (B.isInside(pos) == -1){
        return vector3d(0,0,0);
    }
    Electron2D e_dummy;
    e_dummy.Et = Et;
    e_dummy.pos = pos;
    e_dummy.vel = vel;
    double time_halved = time_interval/2;
    // get k1                                       
    vector3d F_m = (e_dummy.vel % B.getField(pos))*eQMratio;                                       
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();     
    // get k2
    e_dummy.move(time_halved);
    e_dummy.accelerate(k1, time_halved);                                              
    F_m = (e_dummy.vel % B.getField(pos))*eQMratio;                                               
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();    
    // get k3
    e_dummy.vel = vel;
    e_dummy.accelerate(k2, time_halved);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();   
    // get k4
    e_dummy.vel = vel;
    e_dummy.move(time_halved);
    e_dummy.accelerate(k3, time_interval);                                           
    F_m = (e_dummy.vel % B.getField(pos))*eQMratio;  
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

    return (k1 + k2*2 + k3*2 + k4)/6;
}
