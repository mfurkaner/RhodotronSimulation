#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "rhodo.h"
using namespace std;
int STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1.0;
int NUM_OF_ELECTRONS = 100;

void displayHelp(){
 cout << "\nThis is a Rhodotron simulator. Cmdline parameters are:\n\n";
 cout << "-enum number of electrons in a bunch\n";
 cout << "-ph the RF phase (degree)\n";
 cout << "-gt the gun active time (ns)\n";
 cout << "-L1 the total distance outside cavity after traverse1 (m)\n";
 cout << "-L2 the total distance outside cavity after traverse2 (m)\n";
 cout << "-L3 the total distance outside cavity after traverse3 (m)\n";
 cout << "-phopt find the optimal phase lag (degree) -d to get details\n";
 cout << "-magopt get the optimal outside path geometry. Argument is number of magnets\n";
 cout << "-l2opt get the optimal second magnet geometry\n";
 cout << "-sum get the summary of the current settings\n";
 cout << endl;
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

pair<double, double> distout_to_Lrho_pair( double dist_out ){
  pair<double, double> L_rho;
  double mag_guide = (dist_out - R2*(M_PI + 2*MAGNET_ROTATION_R)*tan(MAGNET_ROTATION_R)) / (2 + tan(MAGNET_ROTATION_R)*(M_PI + 2*MAGNET_ROTATION_R) ) ;
  double rho = (dist_out - 2*mag_guide) / (M_PI + 2*MAGNET_ROTATION_R);
  L_rho.first = mag_guide;
  L_rho.second = rho;
  return L_rho;
}

int phase_opt(double Lout1, double Lout2, bool detail){
    double minrms = 1;
    int opt_phase;
    for(int RFphase = -PHASE_SWEEP; RFphase <= PHASE_SWEEP; RFphase++){
      Bunch bunch1(RFphase);
      double t1 = 0;
      bunch1.bunch_gecis_t(t1);
      bunch1.reset_pos();
      if(detail){
        cout << "phase lag : " << RFphase << endl;
        cout<< "Gecis 1) " ; bunch1.print_summary();
      }
      bunch1.reset_pos();
      bunch1.bunch_gecis_d(Lout1);
      if(detail){
        cout<< "Gecis 2) " ; bunch1.print_summary();
      }
      bunch1.reset_pos();
      bunch1.bunch_gecis_d(Lout2);
      if(detail){
        cout<< "Gecis 3) " ; bunch1.print_summary(); cout << endl << endl;
      }
      if( bunch1.E_rms() < minrms ){
        minrms = bunch1.E_rms();
        opt_phase = RFphase;
      }
    }
    cout << setprecision(4) << endl;
    cout << "Optimal phase with the least RMS ( " << minrms << " MeV) after third pass : " << opt_phase << endl;
    return opt_phase;
}

int phase_opt(double Lout1, double Lout2, double Lout3, bool detail){
    double minrms = 1;
    int opt_phase;
    for(int RFphase = -PHASE_SWEEP; RFphase <= PHASE_SWEEP; RFphase++){
      Bunch bunch1(RFphase);
      double t1 = 0;
      bunch1.bunch_gecis_t(t1);
      bunch1.reset_pos();
      if(detail){
        cout << "phase lag : " << RFphase << endl;
        cout<< "Gecis 1) " ; bunch1.print_summary();
      }
      bunch1.reset_pos();
      bunch1.bunch_gecis_d(Lout1);
      if(detail){
        cout<< "Gecis 2) " ; bunch1.print_summary();
      }
      bunch1.reset_pos();
      bunch1.bunch_gecis_d(Lout2);
      if(detail){
        cout<< "Gecis 3) " ; bunch1.print_summary();
      }
      bunch1.reset_pos();
      bunch1.bunch_gecis_d(Lout3);
      if(detail){
        cout<< "Gecis 4) " ; bunch1.print_summary(); cout << endl << endl;
      }
      if( bunch1.E_rms() < minrms ){
        minrms = bunch1.E_rms();
        opt_phase = RFphase;
      }
    }
    cout << setprecision(4) << endl;
    cout << "Optimal phase with the least RMS ( " << minrms << " MeV) after third pass : " << opt_phase << endl;
    return opt_phase;
}

vector<double> mag_opt(double RFphase, int magcount){
  Bunch dummy, bunch(RFphase), max;
  double emax = 0, t_opt = 0, vel_max;
  bunch.bunch_gecis_t(t_opt);
  bunch.reset_pos();
  vector<double> path;
  for(int j = 0 ; j < magcount ; j++){
    vel_max = bunch.e[bunch.index_fastest].get_vel();
    for(double i = 2; i < 9 ; i+=dT_out ){
      dummy = bunch;
      dummy.bunch_gecis_t(i);
      if( emax < dummy.e[dummy.index_fastest].Et ){
        emax = dummy.e[dummy.index_fastest].Et;
        max = dummy;
        t_opt = i;
      }
    }
    bunch = max;
    bunch.reset_pos();
    path.push_back( vel_to_dist(vel_max, t_opt)*ns );
  }

  for(int j = 0; j < magcount ; j++){
    cout << std::setprecision(4) << endl;
    pair<double, double> Lpair = distout_to_Lrho_pair(path.at(j));
    cout << "For the " << j+1 << "th magnet:\nOptimum out path = " << path.at(j) << " m" << endl;
    cout << "Magnet guide = " << Lpair.first;
    cout << " m\nRho = " << Lpair.second << " m"<< endl;
    cout <<  "Max energy = " << bunch.emax_rms.at(j).first;
    cout << " MeV\nRMS = " << bunch.emax_rms.at(j).second << " MeV" << endl;
  }
  cout << endl;
  bunch.print_summary();
  return path;
}


#pragma region ELECTRON
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
        STEPS_TAKEN++;
    }
    enerjiler.push_back(Et);
}
#pragma endregion

//---------------------------bunch
#pragma region BUNCH
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
    cout<<endl<< "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << " MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second << " MeV" << endl;
}

void Bunch::print_summary(){
  cout << "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << " MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second << " MeV" << endl;
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
    else{
      cerr << "CANNOT USE -bunch_gecis_d- FOR THE FIRST PASS!" << endl;
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
    // bir elektronun giriş zamanı = kaviteden çıkış zamanı + dışarıdaki yolda geçirdiği zaman
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

#pragma endregion
