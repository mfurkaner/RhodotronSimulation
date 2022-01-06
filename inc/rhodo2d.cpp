#include "rhodo2d.h"

#pragma region ELECTRON
double Electron2D::get_vel(){
    return c*sqrt(Et*Et-E0*E0)/Et;
}

double Electron2D::get_travel_time(double dist){  // calculate drift time
    return dist/get_vel();
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
    pos += vel*(dt*ns) + acc*(dT*ns)*(dT*ns)/2;
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


void Bunch2D::interact(RFField& E, MagneticField& B, double time, double time_interval, DataStorage& path, bool draw){
    for(int i = 0; i < e.size() ; i++){
        if ( time < i*ns_between){
            continue;
        }
        vector3d acc_E = E.actOn(e[i]);
        vector3d acc_B = B.actOn(e[i]);
        vector3d acc = acc_E + acc_B;

        e[i].move(time_interval/2);
        e[i].accelerate( acc_E + acc_B , time_interval);
        e[i].move(acc_E + acc_B, time_interval/2);

        if( e[i].isinside && e[i].pos.magnitude() > R2){
            e[i].isinside = false;
            e[i].t_giris_cikis[(e[i].t_giris_cikis.size() - 1)].second = time;
        }
        else if( !e[i].isinside && e[i].pos.magnitude() <= R2){
            e[i].isinside = true;
            e[i].t_giris_cikis.push_back(giris_cikis_tpair(time, time));
        }
        if ( draw ){
            path << setprecision(4) << "v: " << e[i].vel << "    E: " << setprecision(6) << e[i].Et - E0 << "   pos: " << e[i].pos << "   acc: "<< acc <<"\n";
        }
        if ( e[i].Et > max_energy ){
            index_fastest = i;
            max_energy = e[i].Et;
        }
    }
    
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
