#include "inc/rhodo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <chrono>


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
  double Lout1=0.808;
  // command line arguments
 for(int i=1; i <argc; i++) {
  if        (std::string(argv[i]) == "-h")  { displayHelp(); exit(0);
  } else if (std::string(argv[i]) == "-L1") { Lout1=atof(argv[i + 1] );
  }
 }


  Bunch bunch;
  double t = 0;
  
  bunch.bunch_gecis_t(t);
  t = bunch.e[bunch.index_fastest].t_giris_cikis.at(0).second;
  double vel_max = bunch.e[bunch.index_fastest].get_vel();
  cout<<endl<< "Gecis 1) Electron with the most energy : " << bunch.index_fastest + 1 << ") " << bunch.emax_rms.at(bunch.pass_count-1).first << " MeV,\tRMS of bunch : " << bunch.emax_rms.at(bunch.pass_count-1).second << " MeV" << endl;
  bunch.reset_pos();


  Bunch dummy, max;
  double emax, t_opt;
/*
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

  cout << "Optimum first out path = " << p1_path << "m with max energy : " << emax1 << " MeV" <<  endl;
  cout << "Optimum second out path = " << p2_path << "m with max energy : " << emax2 << " MeV" <<  endl;
  cout << "Optimum third out path = " << p3_path << "m with max energy : " << emax3 << " MeV" <<  endl;
  */
//  bunch.bunch_gecis_d(0.808);
  bunch.bunch_gecis_d(Lout1);

  //bunch.print_bunch_info();
  cout<<endl<< "Gecis 2) Electron with the most energy : " << bunch.index_fastest + 1 << ") " << bunch.emax_rms.at(bunch.pass_count-1).first << " MeV,\tRMS of bunch : " << bunch.emax_rms.at(bunch.pass_count-1).second << " MeV" << endl;

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Simulation finished in : " << duration.count() << " us" << endl;

  return 0;
}


