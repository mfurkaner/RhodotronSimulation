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
  double Lout1=0.808; //m
  double Lout2=1.080; //m
  double Lout3=1.173; //m
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
  
  bunch.reset_pos();
  bunch.bunch_gecis_d(Lout1);
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
  cout << "For L1 : " << Lout1 << " m, L2opt : " << d_opt << " m";
  
  #pragma endregion

  #pragma region SUMMARY
  /*
  cout<<endl<< "Gecis 1) " ; bunch.print_summary();
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

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << std::setprecision(4);
  cout << endl << "Total steps calculated : " << STEPS_TAKEN << endl;
  cout << "Simulation total run time : " << t << " ns" << endl;
  cout << "Simulation finished in : " << duration.count() << " us" << endl << endl;

  return 0;
}


