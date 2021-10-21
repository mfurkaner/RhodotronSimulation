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

  Bunch bunch;

  double t = 0;
  
  bunch.bunch_gecis(t);
  t = bunch.e[bunch.index_fastest].t_giris_cikis.at(0).second;
  double vel_max = bunch.e[bunch.index_fastest].get_vel();
  
  bunch.reset_pos();

  Bunch dummy, max;
  double emax, t_opt;

  for(double i = 2; i < 9 ; i+=dT_out ){
    dummy = bunch;
    dummy.bunch_gecis(i);
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
    dummy.bunch_gecis(i);
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
    dummy.bunch_gecis(i);
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
  
  cout << endl << endl;
  max.print_bunch_info();

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Simulation finished in : " << duration.count() << " mus" << endl;

  return 0;
}


