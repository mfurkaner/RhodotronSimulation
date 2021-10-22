#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <chrono>

#include "inc/rhodo.h"



using namespace std;
using namespace std::chrono;


// find E field, calculate force,
// calculate acceleration, accelerate particle
// find new relativistic parameters and kinetic energy
// print some quantities
// find new position: are we in E field, drift space of magnet?
// did we exit?

int main (int argc, char *argv[] ) {

  auto start = high_resolution_clock::now();
  double Lout1=0.8104; //m
  double Lout2=1.08326; //m
  double Lout3=1.1705; //m
  double RFphase=0; //degree
  // command line arguments
 for(int i=1; i <argc; i++) {
  if        (std::string(argv[i]) == "-h")  { displayHelp(); exit(0);
  } else if (std::string(argv[i]) == "-L1") { Lout1=atof(argv[i + 1] );
  } else if (std::string(argv[i]) == "-L2") { Lout2=atof(argv[i + 1] );
  } else if (std::string(argv[i]) == "-L3") { Lout3=atof(argv[i + 1] );
  } else if (std::string(argv[i]) == "-ph") { RFphase=atof(argv[i + 1] );
  }
 }

  Bunch bunch(RFphase);
  double t = 0;
  
  bunch.bunch_gecis_t(t);
  t = bunch.e[bunch.index_fastest].t_giris_cikis.at(0).second;
  double vel_max = bunch.e[bunch.index_fastest].get_vel();
  
  #pragma region L2_OPT
  /*
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout1);
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout2);
  bunch.reset_pos();
  Bunch dummy, max;
  double emax = 0, d_opt;
  for(double i = 0.8; i<1.2 ; i+=dD){
    dummy = bunch;
    dummy.bunch_gecis_d(i);
    if( emax < dummy.e[dummy.index_fastest].Et ){
      emax = dummy.e[dummy.index_fastest].Et;
      max = dummy;
      d_opt = i;
    }
  }
  max.print_summary();
  cout << "For L1 : " << Lout1 << " m, L2 : " << Lout2 << " m, L3opt : "<<d_opt << " m";
  */
  #pragma endregion

  #pragma region SUMMARY
  /*
  cout << endl << "Summary for " << RFphase << " degrees of phase lag" << endl;

  cout<< "Gecis 1) " ; bunch.print_summary();
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout1);
  cout<<endl<< "Gecis 2) " ; bunch.print_summary();
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout2);
  cout<<endl<< "Gecis 3) " ; bunch.print_summary();
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout3);
  cout<<endl<< "Gecis 4) " ; bunch.print_summary();

  pair<double, double> Lout1_pair = distout_to_Lrho_pair(Lout1);
  pair<double, double> Lout2_pair = distout_to_Lrho_pair(Lout2);
  cout << std::setprecision(4) << endl;
  cout << "For the first magnet : " << Lout1 <<" m, magnet guide : " << Lout1_pair.first << " m, rho : " << Lout1_pair.second << " m"<< endl;
  cout << "For the second magnet : " << Lout2 <<" m, magnet guide : " << Lout2_pair.first << " m, rho : " << Lout2_pair.second << " m"<< endl;

  t = bunch.e[bunch.e_count - 1].t_giris_cikis.at(bunch.pass_count - 1).second;
  */
  #pragma endregion

  #pragma region MAGNET_OPT
/*
  Bunch dummy, max;
  double emax, t_opt;
  bunch.reset_pos();
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
  double p1_path = vel_to_dist(vel_max, t_opt)*ns ;
  vel_max = bunch.e[bunch.index_fastest].get_vel();
  double emax1 = emax - E0;

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
  double p2_path = vel_to_dist(vel_max, t_opt)*ns ;
  vel_max = bunch.e[bunch.index_fastest].get_vel();
  double emax2 = emax - E0;

  for(double i = 2; i < 9 ; i+=dT_out ){
    dummy = bunch;
    dummy.bunch_gecis_t(i);
    if( emax < dummy.e[dummy.index_fastest].Et ){
      emax = dummy.e[dummy.index_fastest].Et;
      max = dummy;
      t_opt = i;
    }
  }

  double p3_path = vel_to_dist(vel_max, t_opt)*ns ;
  double emax3 = emax - E0;

  cout << "Optimum first out path = " << p1_path << " m with max energy : " << emax1 << " MeV" <<  endl;
  cout << "Optimum second out path = " << p2_path << " m with max energy : " << emax2 << " MeV" <<  endl;
  cout << "Optimum third out path = " << p3_path << " m with max energy : " << emax3 << " MeV" <<  endl;
  
  max.print_summary();
*/
#pragma endregion

#pragma region PHASE_OPT
/*
double minrms = 1;
int opt_phase;
for(RFphase = -10; RFphase <= 10; RFphase++){
  Bunch bunch1(RFphase);
  double t1 = 0;
  bunch1.bunch_gecis_t(t1);
  bunch1.reset_pos();
  cout << "phase lag : " << RFphase << endl;
  cout<< "Gecis 1) " ; bunch1.print_summary();
  bunch1.reset_pos();
  bunch1.bunch_gecis_d(Lout1);
  cout<< "Gecis 2) " ; bunch1.print_summary();
  bunch1.reset_pos();
  bunch1.bunch_gecis_d(Lout2);
  if( bunch1.E_rms() < minrms ){
    minrms = bunch1.E_rms();
    opt_phase = RFphase;
  }
  cout<< "Gecis 3) " ; bunch1.print_summary();
  bunch1.reset_pos();
  bunch1.bunch_gecis_d(Lout3);
  cout<< "Gecis 4) " ; bunch1.print_summary(); cout << endl << endl;
}
cout << "Phase with the least RMS after third pass : " << opt_phase;
*/
#pragma endregion

#pragma region SINAN

  cout << GUN_ACTIVE_TIME << " ns aktif gun için" << endl << endl << "-optimum phase lag : " << RFphase << " derece" << endl; 
  cout << "\t*Gecis 1)"<< endl << "\t\t";
  bunch.print_summary();
  Bunch dummy, max;
  double emax, t_opt;
  bunch.reset_pos();
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
  double p1_path = vel_to_dist(vel_max, t_opt)*ns ;
  double p1_L = distout_to_Lrho_pair(p1_path).first;
  double p1_rho = distout_to_Lrho_pair(p1_path).second;
  vel_max = bunch.e[bunch.index_fastest].get_vel();

  cout << "\t\t-Disaridaki toplam yol : " << p1_path << " m" << endl;
  cout << "\t\t-Manyetik alan dışında drift : " << p1_L << " m" << endl;
  cout << "\t\t-Manyetik alanda yaricap : " << p1_rho << " m" << endl;
  cout << "\t\t-İlk elektronun dışarıda geçirdiği süre : " << max.e[0].t_giris_cikis.at(0).second - max.e[0].t_giris_cikis.at(0).first << " ns" << endl;
  cout << "\t\t-Ortadaki elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(0).second - max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(0).first << " ns" << endl;
  cout << "\t\t-Son elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(0).second - max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(0).first << " ns" << endl;

  cout << "\t*Gecis 2)"<< endl << "\t\t";
  max.print_summary();

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
  double p2_path = vel_to_dist(vel_max, t_opt)*ns ;
  double p2_L = distout_to_Lrho_pair(p2_path).first;
  double p2_rho = distout_to_Lrho_pair(p2_path).second;
  vel_max = bunch.e[bunch.index_fastest].get_vel();

  cout << "\t\t-Disaridaki toplam yol : " << p2_path << " m" << endl;
  cout << "\t\t-Manyetik alan dışında drift : " << p2_L << " m" << endl;
  cout << "\t\t-Manyetik alanda yaricap : " << p2_rho << " m" << endl;
  cout << "\t\t-İlk elektronun dışarıda geçirdiği süre : " << max.e[0].t_giris_cikis.at(1).second - max.e[0].t_giris_cikis.at(1).first << " ns" << endl;
  cout << "\t\t-Ortadaki elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(1).second - max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(1).first << " ns" << endl;
  cout << "\t\t-Son elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(1).second - max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(1).first << " ns" << endl;
  
  cout << "\t*Gecis 3)"<< endl << "\t\t";
  max.print_summary();

  for(double i = 2; i < 9 ; i+=dT_out ){
    dummy = bunch;
    dummy.bunch_gecis_t(i);
    if( emax < dummy.e[dummy.index_fastest].Et ){
      emax = dummy.e[dummy.index_fastest].Et;
      max = dummy;
      t_opt = i;
    }
  }

  double p3_path = vel_to_dist(vel_max, t_opt)*ns ;
  double p3_L = distout_to_Lrho_pair(p3_path).first;
  double p3_rho = distout_to_Lrho_pair(p3_path).second;

  cout << "\t\t-Disaridaki toplam yol : " << p3_path << " m" << endl;
  cout << "\t\t-Manyetik alan dışında drift : " << p3_L << " m" << endl;
  cout << "\t\t-Manyetik alanda yaricap : " << p3_rho << " m" << endl;
  cout << "\t\t-İlk elektronun dışarıda geçirdiği süre : " << max.e[0].t_giris_cikis.at(2).second - max.e[0].t_giris_cikis.at(2).first << " ns" << endl;
  cout << "\t\t-Ortadaki elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(2).second - max.e[NUM_OF_ELECTRONS/2].t_giris_cikis.at(2).first << " ns" << endl;
  cout << "\t\t-Son elektronun dışarıda geçirdiği süre : " << max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(2).second - max.e[NUM_OF_ELECTRONS - 1].t_giris_cikis.at(2).first << " ns" << endl;
  
  cout << "\t*Gecis 4)"<< endl << "\t\t";
  max.print_summary();


  
#pragma endregion



  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << std::setprecision(4);
  cout << endl << "Total steps calculated : " << STEPS_TAKEN << endl;
  cout << "Simulation total run time : " << max.e[NUM_OF_ELECTRONS-1].t_giris_cikis.at(3).second << " ns" << endl;
  cout << "Simulation finished in : " << duration.count() << " us" << endl << endl;

  return 0;
}


