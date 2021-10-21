#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "rhodo.h"
using namespace std;

void displayHelp(){
 cout << "This is a Rhdotron simulator. Cmdline parameters are:\n";
 cout << "-L1 the total distance outside cavity after traverse1 (m)\n";
 cout << "-L2 the total distance outside cavity after traverse2 (m)\n";
 cout << "-L3 the total distance outside cavity after traverse3 (m)\n";
 cout << "-ph the RF phase (degree)\n";
}


//r is in mm
//time is in ns
//phase in radians
double Eradial(double r, double time, double phase){
  double w=2*M_PI*freq/1000;

  double E_zero = sin(w*time+phase)*Emax*Emax_pos/r;

  double res = E_zero*((r < -R1*1000)*(r > -R2*1000) | (r < R2*1000)*(r > R1*1000));

  return res;
}

double vel_to_dist(double vel, double t){
  return vel*t;
}

double dist_to_time(double dist, double vel){
  return dist/vel;
}

//---------------------------------electron functions
double Electron::get_vel(){
    return c*sqrt(Et*Et-E0*E0)/Et;
}

double Electron::get_travel_time(double dist){  // calculate drift time
    return dist/get_vel();
}

void Electron::print_electron_info(){
    cout<<std::setprecision(4);
    for(int i = 0; i < t_giris_cikis.size() ; i++ ){
        cout << "\tGecis " << i+1 << ") " << "Enerji : " << enerjiler.at(i)-E0 << " MeV, giris zamani : " << t_giris_cikis.at(i).first << " ns, cikis zamani : " << t_giris_cikis.at(i).second << " ns" << endl;
    }
}

void Electron::e_gecis(double &t){
    for(; r_pos >= -R2 && r_pos <= R2 ; t+=dT){
        double vel = get_vel();
        double RelBeta  = vel/c;
        double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

        double ef=Eradial(this->r_pos*1000,t,RFphase*deg_to_rad); // convert position to mm
        double acc=ef*1E6*eQMratio/(RelGamma*RelGamma*RelGamma); 

        r_pos += vel * dT*ns + 1/2*acc*(dT*ns)*(dT*ns);
        vel=vel+acc*dT*ns;
        RelBeta  = vel/c;
        RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
        Et=RelGamma*E0; 
    }
    enerjiler.push_back(Et);
}
//---------------------------bunch
void Bunch::reset_pos(){
    for(int i = 0; i < e_count ; i++){
        e[i].r_pos = -R2;
    }
}

void Bunch::print_bunch_info(){
    cout<<std::setprecision(4);
    for(int i = 0; i < e_count;i++){
        if( i == index_fastest ){
        cout << "** ";
        }
        cout << "Electron " << i+1 << ":" << endl;
        e[i].print_electron_info();
    }
    cout<<endl<< "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << "MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second << " MeV" << endl;
}

void Bunch::print_summary(){
  cout<<endl<< "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << "MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second << " MeV" << endl;
}

void Bunch::bunch_gecis_t(double &t_delay_of_max){
    bool isFirstPass = pass_count == 0;
    isFirstPass ? bunch_ilk_gecis(t_delay_of_max) : bunch_nth_gecis_t(t_delay_of_max);
    max_energy_rms_pair me_rms;
    me_rms.first = e[index_fastest].Et - E0;
    me_rms.second = E_rms();
    emax_rms.push_back( me_rms );
}

void Bunch::bunch_gecis_d(double dist_out){
    if( pass_count ){
      bunch_nth_gecis_d(dist_out/ns);   // convert to nm
      max_energy_rms_pair me_rms;
      me_rms.first = e[index_fastest].Et - E0;
      me_rms.second = E_rms();
      emax_rms.push_back( me_rms );
    }
}

void Bunch::bunch_ilk_gecis(double &t){
    double t_e[e_count];
    giris_cikis_tpair tpair;
    double emax = 0;
    for(int i = 0 ; i < e_count ; i++){
        t_e[i] = t + i * ns_between;
        tpair.first = t_e[i];
        e[i].RFphase=RFphase_bunch;
        e[i].e_gecis(t_e[i]);
        tpair.second = t_e[i];
        e[i].t_giris_cikis.push_back(tpair);
        if(emax < e[i].Et){
            emax = e[i].Et;
            index_fastest = i;
        }
    }
    pass_count++;
}

// ilk geçiş için bu kullanılmayacak. for loop içerisinde pass_count kontrolü yok
void Bunch::bunch_nth_gecis_t(double t_delay_of_max){
  double t_e[e_count];
  giris_cikis_tpair tpair;
  double emax = 0;
  // find the distance bunch needs to travel out of the cavity given the delay of the peak energy electron
  double dist_out = vel_to_dist(e[index_fastest].get_vel(), t_delay_of_max);

  for(int i = 0 ; i < e_count ; i++){
    // bir elektronun giriş zamanı = bir öncekinden çıkış zamanı + dışarıdaki yolda geçirdiği zaman
    t_e[i] = e[i].t_giris_cikis.at(pass_count-1).second + e[i].get_travel_time(dist_out);
    tpair.first = t_e[i];
    e[i].e_gecis(t_e[i]);
    tpair.second = t_e[i];
    e[i].t_giris_cikis.push_back(tpair);
    if(emax < e[i].Et){
      emax = e[i].Et;
      index_fastest = i;
    }
  }
  pass_count++;
}

void Bunch::bunch_nth_gecis_d(double dist_out){ // distance must be in nm
  double t_e[e_count];
  giris_cikis_tpair tpair;
  double emax = 0;
  // find the distance bunch needs to travel out of the cavity given the delay of the peak energy electron

  for(int i = 0 ; i < e_count ; i++){
    // bir elektronun giriş zamanı = bir öncekinden çıkış zamanı + dışarıdaki yolda geçirdiği zaman
    t_e[i] = e[i].t_giris_cikis.at(pass_count-1).second + e[i].get_travel_time(dist_out); // units are ns
    tpair.first = t_e[i];
    e[i].e_gecis(t_e[i]);
    tpair.second = t_e[i];
    e[i].t_giris_cikis.push_back(tpair);
    if(emax < e[i].Et){
      emax = e[i].Et;
      index_fastest = i;
    }
  }
  pass_count++;
}

double Bunch::E_ave(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += e[i].Et - E0;
  }
  return result/e_count;
}

double Bunch::E_rms(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += (e[i].Et - E0 - E_ave()) * (e[i].Et - E0 - E_ave());
  }
  return sqrt(result/e_count);
}
