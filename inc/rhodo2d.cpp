#include "rhodo2d.h"

std::ostream& operator<<(std::ostream& stream, ElectronLog& log){
    (stream << log.time << "   " << log.energy << "   ") <= log.pos;
    (stream << "   ") <= log.vel;
    return stream;
}

#pragma region ELECTRON
double Electron2D::get_vel(){
    return c*sqrt(Et*Et-E0*E0)/Et;
}


void Electron2D::print_electron_info(){
    cout<<std::setprecision(4);
    for(int i = 0; i < t_giris_cikis.size() ; i++ ){
        cout << "\tGecis " << i+1 << ") " << "Enerji : " << enerjiler.at(i)-E0 << " MeV, giris zamani : " << t_giris_cikis.at(i).first << " ns, cikis zamani : " << t_giris_cikis.at(i).second << " ns" << endl;
    }
    cout << "Energy : " << Et - E0 << "    Position : " << pos << "    Velocity : " << vel << endl;
}

void Electron2D::move(double dt){
    pos += vel*(dt*ns);
}

void Electron2D::move(vector3d acc, double dt){
    pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2;
}

void Electron2D::move(vector3d acc, vector3d jerk, double dt){
    pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2 + jerk*(dt*ns*dt*ns*dt*ns)/6 ;
}

void Electron2D::accelerate(vector3d acc, vector3d jerk, double dt){
    vel += acc*(dt*ns) + jerk*(dt*ns*dt*ns)/2;  
    Et = gamma()*E0;                                                
}

void Electron2D::accelerate(vector3d acc, double dt){
    vel += acc*(dt*ns);  
    Et = gamma()*E0;                                                
}




#pragma endregion ELECTRON

#pragma region BUNCH
void Bunch2D::reset(){
    for(int i = 0; i < e_count ; i++){
        e[i].pos = vector3d(-R2, 0, 0);
        e[i].Et = E0 + Ein/1000;
        e[i].vel = vector3d(e[i].get_vel(), 0, 0);
    }
}

double Bunch2D::E_ave(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += e[i].Et - E0;
  }
  return result/e_count;
}

double Bunch2D::E_rms(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += (e[i].Et - E0 - E_ave()) * (e[i].Et - E0 - E_ave());
  }
  return sqrt(result/e_count);
}

#define RK2
void Bunch2D::interact(RFField& E, MagneticField& B, double time, double time_interval){
    for(int i = 0; i < e.size() ; i++){
        if ( time < i*ns_between){
            continue;
        }

        #ifdef LF
        vector3d acc_E = E.actOn(e[i]);
        vector3d acc_B = B.actOn(e[i]);
        vector3d acc = acc_E + acc_B;
        vector3d jerk = B.getJerk(e[i].pos, e[i].vel, acc);
        if ( time > 6 && time < 7){
            vector3d v = (jerk*time_interval*time_interval*ns*ns/2);
            cout << v << "\n";
        }
        e[i].move( acc, jerk, time_interval/2);
        e[i].accelerate( acc, jerk, time_interval);
        e[i].move( acc, jerk, time_interval/2);
        #endif

        #ifdef RK2
        Electron2D e_dummy;
        e_dummy.Et = e[i].Et;
        e_dummy.pos = e[i].pos;
        e_dummy.vel = e[i].vel;

        // Calculate k1
        vector3d acc_E = E.actOn(e_dummy);
        vector3d acc_B = B.actOn(e_dummy);
        vector3d acc = acc_E + acc_B;
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc, time_interval);
        vector3d pos_k1 = e_dummy.pos, vel_k1 = e_dummy.vel;

        // Calculate k2
        e_dummy.pos = (e[i].pos + pos_k1)/2;
        e_dummy.vel = (e[i].vel + vel_k1)/2;
        e_dummy.Et = e_dummy.gamma()*E0;
        E.update(time + time_interval/2);

        acc_E = E.actOn(e_dummy);
        acc_B = B.actOn(e_dummy);
        acc = acc_E + acc_B;
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc, time_interval);
        vector3d pos_k2 = e_dummy.pos, vel_k2 = e_dummy.vel;

        // Calculate k3
        e_dummy.pos = (e[i].pos + pos_k2)/2;
        e_dummy.vel = (e[i].vel + vel_k2)/2;
        e_dummy.Et = e_dummy.gamma()*E0;
        E.update(time + time_interval/2);

        acc_E = E.actOn(e_dummy);
        acc_B = B.actOn(e_dummy);
        acc = acc_E + acc_B;
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc, time_interval);
        vector3d pos_k3 = e_dummy.pos, vel_k3 = e_dummy.vel;

        // Calculate k4
        E.update(time + time_interval);

        acc_E = E.actOn(e_dummy);
        acc_B = B.actOn(e_dummy);
        acc = acc_E + acc_B;
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc, time_interval);
        vector3d pos_k4 = e_dummy.pos, vel_k4 = e_dummy.vel;

        e[i].pos = (pos_k1 + pos_k2*2 + pos_k3*2 + pos_k4)/6;
        e[i].vel = (vel_k1 + vel_k2*2 + vel_k3*2 + vel_k4)/6;
        e[i].Et = e[i].gamma()*E0;

        #endif
    }
}


void Bunch2D::interactBonly(MagneticField& B, double time, double time_interval){
    for(int i = 0; i < e.size() ; i++){
        if ( time < i*ns_between){
            continue;
        }
        #ifdef LF
        vector3d acc_B = B.actOn(e[i], false);
        vector3d acc = acc_B;
        e[i].move( acc,time_interval/2);
        e[i].accelerate( acc, time_interval);
        e[i].move( acc, time_interval/2);
        #endif

        #ifdef RK1
        
        std::vector<vector3d> coeff = B.actOnAndGetRungeKuttaCoef(e[i], time_interval);
        e[i].pos = coeff[0];
        e[i].vel = coeff[1];
        e[i].Et = e[i].gamma()*E0;
        #endif

        #ifdef RK2
        Electron2D e_dummy;
        e_dummy.Et = e[i].Et;
        e_dummy.pos = e[i].pos;
        e_dummy.vel = e[i].vel;

        // Calculate k1
        vector3d acc_B = B.actOn(e_dummy);
        if(acc_B == vector3d(0,0,0)){
            e[i].move(time_interval);
            continue;
        }
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc_B, time_interval);
        vector3d pos_k1 = e_dummy.pos, vel_k1 = e_dummy.vel;

        // Calculate k2
        e_dummy.pos = (e[i].pos + pos_k1)/2;
        e_dummy.vel = (e[i].vel + vel_k1)/2;
        e_dummy.Et = e_dummy.gamma()*E0;

        acc_B = B.actOn(e_dummy);
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc_B, time_interval);
        vector3d pos_k2 = e_dummy.pos, vel_k2 = e_dummy.vel;

        // Calculate k3
        e_dummy.pos = (e[i].pos + pos_k2)/2;
        e_dummy.vel = (e[i].vel + vel_k2)/2;
        e_dummy.Et = e_dummy.gamma()*E0;

        acc_B = B.actOn(e_dummy);
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc_B, time_interval);
        vector3d pos_k3 = e_dummy.pos, vel_k3 = e_dummy.vel;

        // Calculate k4
        acc_B = B.actOn(e_dummy);
        e_dummy.move(time_interval);
        e_dummy.accelerate(acc_B, time_interval);
        vector3d pos_k4 = e_dummy.pos, vel_k4 = e_dummy.vel;

        e[i].pos = (pos_k1 + pos_k2*2 + pos_k3*2 + pos_k4)/6;
        e[i].vel = (vel_k1 + vel_k2*2 + vel_k3*2 + vel_k4)/6;
        e[i].Et = e[i].gamma()*E0;

        #endif
    }
}

void Bunch2D::divide(unsigned int num){
    for(int i = 0; i < num ; i++){
        Bunch2D sub( i ? e_count/num : e_count/num + e_count%num);
        // settings
        for(int j = 0 ; j < sub.e_count ; j++){
            sub.e.push_back( e.at( (i ? (e_count/num)*i + e_count%num : 0)  + j ) );
        }
        subBunchs.push_back(sub);
    }
}

Bunch2D& Bunch2D::subBunch(unsigned int index){
    if(subBunchs.size() > index){
        return subBunchs.at(index);
    }
    return *this;
}

vector<Bunch2D*> Bunch2D::subBunchPtr(){
    vector<Bunch2D*> ptrv;
    for(int i = 0; i < subBunchs.size() ; i++ ){
        ptrv.push_back(&subBunchs[i]);
    }
    return ptrv;
}

void Bunch2D::print_summary(){
  cout << "Electron with the most energy : " << index_fastest + 1 << ") " << e[index_fastest].Et - E0 << " MeV,\tE_ave of bunch : "<< E_ave() << " MeV,\tRMS of bunch : " << E_rms() <<  " MeV" << endl;
}

void Bunch2D::print_bunch_info(){
    cout<<std::setprecision(4);
    for(int i = 0; i < e_count;i++){
        if( i == index_fastest ){
        cout << "** ";
        }
        cout << "Electron " << i+1 << ":" << endl;
        e[i].print_electron_info();
    }
    print_summary();
}

#pragma endregion BUNCH
