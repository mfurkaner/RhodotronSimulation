#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include "rhodo.h"

using namespace std;using namespace std::chrono;


uint64_t STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1.0;
int NUM_OF_ELECTRONS = 250;
bool NOTIFICATIONS = false;
double dT = 0.01;
double dT_out = 0.01;

bool MULTI_THREAD = false;
int MAX_THREAD_COUNT = 1;


void displayHelp(){
 cout << "\nThis is a Rhodotron simulator. Cmdline parameters are:\n\n";
 cout << "-enum number of electrons in a bunch\n";
 cout << "-ph the RF phase (degree)\n";
 cout << "-gt the gun active time (ns)\n";
 cout << "-dt time pass for each step inside the cavity (ns)\n";
 cout << "-dto time pass for each step outside the cavity (ns)\n";
 cout << "-L1 the total distance outside cavity after traverse1 (mm)\n";
 cout << "-L2 the total distance outside cavity after traverse2 (mm)\n";
 cout << "-L3 the total distance outside cavity after traverse3 (mm)\n";
 cout << "-phopt find the optimal phase lag (degree) -d to get details\n";
 cout << "-magopt get the optimal outside path geometry. Argument is number of magnets -d to get details\n";
 cout << "-l2opt get the optimal second magnet geometry\n";
 cout << "-sum get the summary of the current settings\n";
 cout << "-n enable terminal-notifier notifications\n";
 cout << "-mt enable multi-threading for -magopt. Argument is number of threads to use (default is 2)\n";
 cout << endl;
}



//r is in mm
//time is in ns
//phase in radians
double Eradial(double r, double time, double phase){
  double w=2*M_PI*freq/1000;

  double E_zero = sin(w*time+phase)*Emax*Emax_pos/r;
//  double E_zero = sin(w*time+phase)*Emax/r;

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

#pragma region OPT
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


void mag_sweep_wt(Bunch &bunch, double &t_opt, double sweep_min, double sweep_max){
  Bunch dummy, max;
  double emax = 0;
  for(double i = sweep_min; i < sweep_max ; i+=dT_out){
    dummy = bunch;
    dummy.bunch_gecis_t(i);
    if( emax < dummy.e[dummy.index_fastest].Et ){
      emax = dummy.e[dummy.index_fastest].Et;
      max = dummy;
      t_opt = i;
    }
  }
  bunch = max;
}

struct thread_arg
{
  Bunch *bunch;
  double *t_opt;
  double sweep_min;
  double sweep_max;
};

void* mg_sw_wt_mt(void* threadargs){
  thread_arg *arg = (thread_arg*)threadargs;
  mag_sweep_wt(*arg->bunch, *arg->t_opt, arg->sweep_min, arg->sweep_max);
  pthread_exit(NULL);
}

Bunch mag_sweep(Bunch bunch, double &t_opt){
  mag_sweep_wt(bunch, t_opt, MAGNET_TIME_SWEEP_MIN, MAGNET_TIME_SWEEP_MAX);
  return bunch;
}

Bunch mag_sweep_mt(Bunch bunch, double &t_opt){
  Bunch maxs[MAX_THREAD_COUNT];
  double t_opts[MAX_THREAD_COUNT-1];
  pthread_t t[MAX_THREAD_COUNT-1];
  thread_arg t_args[MAX_THREAD_COUNT];
  void* status[MAX_THREAD_COUNT-1];
  double start = MAGNET_TIME_SWEEP_MIN;
  double add = (MAGNET_TIME_SWEEP_MAX - MAGNET_TIME_SWEEP_MIN)/MAX_THREAD_COUNT;
  for(int i = 0; i< MAX_THREAD_COUNT-1; i++){
    maxs[i+1] = bunch;
    t_args[i].bunch = &maxs[i+1];
    t_args[i].t_opt = &t_opts[i];
    t_args[i].sweep_min = start;
    t_args[i].sweep_max =  start + add - dT_out;
    start += add;
    pthread_create(&t[i], NULL, mg_sw_wt_mt, &t_args[i]);
  }
  maxs[0] = bunch;
  mag_sweep_wt( maxs[0], t_opt, start, MAGNET_TIME_SWEEP_MAX );

  for( int i = 0; i< MAX_THREAD_COUNT - 1; i++){
    pthread_join(t[i], &status[i]);
  }
  int maxe_index = 0;
  double emax = maxs[0].e[maxs[0].index_fastest].Et;
  for(int i = 1; i< MAX_THREAD_COUNT ; i++){
      if(emax < maxs[i].e[maxs[i].index_fastest].Et){
        emax = maxs[i].e[maxs[i].index_fastest].Et;
        maxe_index = i;
        t_opt = t_opts[i-1];
      }
  }
  return maxs[maxe_index];
}

vector<double> mag_opt(double RFphase, int magcount, bool detail){
  Bunch dummy, bunch(RFphase), max;
  double t_opt = 0, vel_max;
  bunch.bunch_gecis_t(t_opt);
  bunch.reset_pos();
  vector<double> path;
  for(int j = 0 ; j < magcount ; j++){
    if( MULTI_THREAD && MAX_THREAD_COUNT > 1){
      bunch = mag_sweep_mt(bunch, t_opt);
    }
    else{
      bunch = mag_sweep(bunch, t_opt);
    }
    vel_max = bunch.e[bunch.index_fastest].get_vel();
    bunch.reset_pos();
    path.push_back( vel_to_dist(vel_max, t_opt)*ns );
    if ( NOTIFICATIONS ) {
      string mes = "terminal-notifier -message \"MAG-";
      mes += to_string(j+1);
      mes += " PASS complete\" -title \"Rhodotron Simulation\"";
      system(mes.c_str());
    }
  }
  

  for(int j = 0; j < magcount ; j++){
    cout << std::setprecision(5) << endl;
    pair<double, double> Lpair = distout_to_Lrho_pair(path.at(j));
    cout << "For the " << j+1 << "th magnet:\nOptimum out path = " << path.at(j) << " m" << endl;
    cout << "Magnet guide = " << Lpair.first;
    cout << " m\nRho = " << Lpair.second << " m"<< endl;
    if(detail){
      cout << "Drift time of the first electron in the bunch : " << bunch.e[j].t_giris_cikis.at(j).second - bunch.e[j].t_giris_cikis.at(j).first << " ns" << endl;
      cout << "Drift time of the middle electron in the bunch : " << bunch.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(j).second - bunch.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(j).first << " ns" << endl;
      cout << "Drift time of the last electron in the bunch : " << bunch.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(j).second - bunch.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(j).first << " ns" << endl;
    }
    cout << "Max energy = " << bunch.emax_rms.at(j).first;
    cout << " MeV\nRMS = " << bunch.emax_rms.at(j).second << " MeV" << endl;
  }
  cout << endl << "After the optimized magnet passes :\n\n";
  bunch.print_summary();
  return path;
}




#pragma endregion OPT


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
        double RelGamma;

        #ifdef FAST_INVERSE_SQRT
        RelGamma = Q_rsqrt(1.0 - RelBeta*RelBeta);
        #else
        RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
        #endif 


        double ef=Eradial(this->r_pos*1000,t,RFphase*deg_to_rad); // convert position to mm
        double acc=ef*1E6*eQMratio/(RelGamma*RelGamma*RelGamma); 

        r_pos += vel * dT*ns + 1/2*acc*(dT*ns)*(dT*ns);
        vel=vel+acc*dT*ns;
        RelBeta  = vel/c;
        #ifdef FAST_INVERSE_SQRT
        RelGamma = Q_rsqrt(1.0 - RelBeta*RelBeta);
        #else
        RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
        #endif 
        Et=RelGamma*E0; 
        STEPS_TAKEN++;
    }
    enerjiler.push_back(Et);
}
#pragma endregion ELECTRON

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
    cout<<endl<< "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << " MeV,\tE_ave of bunch : "<< E_ave() << " MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second << " MeV" << endl;
}

void Bunch::print_summary(){
  cout << "Electron with the most energy : " << index_fastest + 1 << ") " << emax_rms.at(pass_count-1).first << " MeV,\tE_ave of bunch : "<< E_ave() << " MeV,\tRMS of bunch : " << emax_rms.at(pass_count-1).second <<  " MeV" << endl;
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
