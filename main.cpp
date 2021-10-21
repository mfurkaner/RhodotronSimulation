#include "inc/rhodo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <string.h>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

double Eradial(double r, double time, double phase);

double bir_gecis(double r_pos, double vel, double Et, double t);

double gecis(double r_pos, double vel, double Et, double &t);


int main (int argc, char *argv[] ) {

  auto start = high_resolution_clock::now();

  double r_pos, vel;
  double Relbeta;

  double Et=E0 + Ein/1000; // total energy in MeV
  r_pos=-R2;

  double t = 0;

// find E field, calculate force,
// calculate acceleration, accelerate particle
// find new relativistic parameters and kinetic energy
// print some quantities
// find new position: are we in E field, drift space of magnet?
// did we exit?
  

  Et = gecis(r_pos, vel, Et, t);
  cout<<std::setprecision(3);
  cout << "İlk gecis sonrasi enerji : " << Et-E0 << " MeV.  Cikis zamani : " << t << " ns" << endl;
  double t_dum = t;
  double t_opt;
  double maxE = 0;
  for(double i = 2; i < 9; i += dT_out){
    t_dum += i;
    double enow = gecis(r_pos, vel, Et, t_dum);
    if( enow > maxE ){
      maxE = enow;
      t_opt = i;
    }
    t_dum = t;
  }
  cout<<std::setprecision(4);
  cout << "İkinci geçiş sonrası maksimum enerji E = " << maxE - E0 << " MeV.  Dışarıda geçirdiği zaman : " << t_opt << " ns" << endl;

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Simulation finished in : " << duration.count() << " mus" << endl;

  return 0;
}


double Eradial(double r, double time, double phase){
//r is in mm
//time is in ns, freq in MHz, so -9+6 = -3
//phase in radians

  double w=2*M_PI*freq/1000;

  double E_zero = sin(w*time+phase)*Emax*Emax_pos/r;

//  double res=( r < -R1*1000 && r > -R2*1000 ) ? E_zero : 0;
  double res=( r < -R1*1000) *(r > -R2*1000 ) * E_zero;
       res +=( r <  R2*1000) *(r >  R1*1000 ) * E_zero ;

  return res;
}

double bir_gecis(double r_pos, double vel, double Et, double t){
  for(; t<SimuTime; t+=dT){
    vel = c*sqrt(Et*Et-E0*E0)/Et;
    double RelBeta  = vel/c;
    double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

    double ef=Eradial(r_pos*1000,t,RFphase*deg_to_rad); // convert position to mm
    //cout<<std::setprecision(3);
    //cout << "Beta i:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tE:"<< ef<<"(MV/m) \tTime:"<< t<< "ns\t";
    double acc=ef*1E6*eQMratio/(RelGamma*RelGamma*RelGamma); 

    r_pos = r_pos + vel * dT*ns + 1/2*acc*(dT*ns)*(dT*ns);
    vel=vel+acc*dT*ns;
    RelBeta  = vel/c;
    RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
    Et=RelGamma*E0; 
    //cout << "Beta f:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tT:"<< Et-E0<<"(MeV) \n";
  }
  return Et;
}

double gecis(double r_pos, double vel, double Et, double &t){

  for(; r_pos >= -R2 && r_pos <= R2 ; t+=dT){
    vel = c*sqrt(Et*Et-E0*E0)/Et;
    double RelBeta  = vel/c;
    double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

    double ef=Eradial(r_pos*1000,t,RFphase*deg_to_rad); // convert position to mm

    //cout<<std::setprecision(3);
    //cout << "Beta i:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tE:"<< ef<<"(MV/m) \tTime:"<< t<< "ns\t";
    double acc=ef*1E6*eQMratio/(RelGamma*RelGamma*RelGamma); 

    r_pos = r_pos + vel * dT*ns + 1/2*acc*(dT*ns)*(dT*ns);
    vel=vel+acc*dT*ns;
    RelBeta  = vel/c;
    RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
    Et=RelGamma*E0; 
    //cout << "Beta f:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tT:"<< Et-E0<<"(MeV) \n";
  }
  return Et;
}
