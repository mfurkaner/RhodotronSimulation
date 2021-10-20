#include "inc/rhodo.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <string.h>

using namespace std;


double Eradial(double r, double time, double phase){

//r is in mm
//time is in ns, freq in MHz, so -9+6 = -3
//phase in radians

 double Emax=0.960000;   // MVolts/m
 double freq=107.5; //MHz 
 double w=2*M_PI*freq/1000;
 //cout <<" r:"<<r<<" t:"<<time<<" ph:"<<phase<<"\n";

 double res=0;
        res+=sin(w*time+phase)*((Emax/(1+(-200-r)/200))*(r < -188)*(r > -753)
                               -(Emax/(1+(r-200)/200))*(r<753)*(r>188) );

//      res+=cos(w*time+phase)*((Emax/(1+(740+r)/200))*(r < -188)*(r > -753)
//                             +(Emax/(1+(740-r)/200))*(r<753)*(r>188) );
 return -res;
} 


int main (int argc, char *argv[] ) {

double Ein=40;   //keV
double RFphase=15; // degrees
double dT=0.1; //ns
double R1=0.1882; //m
double R2=0.753; //m
double SimuTime=10; //ns
double L1=0.4; // m, distane to first magnet
double Rho1=0.23; //m, radius of the first magnet
double L2=0.4; // m, distane to second magnet
double Rho2=0.23; //m, radius of the second magnet
double r_pos, vel, r_acc;
double Relbeta, RelGamma;

double dist=R2-R1;
double Et=E0 + Ein/1000; // total energy in MeV
r_pos=-R2;

// find E field, calculate force,
// calculate acceleration, accelerate particle
// find new relativistic parameters and kinetic energy
// print some quantities
// find new position: are we in E field, drift space of magnet?
// did we exit?

for (double t=0; t<SimuTime; t+=dT){
  vel = c*sqrt(Et*Et-E0*E0)/Et;
  double RelBeta  = vel/c;
  double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

  double ef=Eradial(r_pos*1000,t,0); // convert position to mm
  cout<<std::setprecision(3);
  cout << "Beta i:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tE:"<< ef<<"(MV/m) \tTime:"<< t<< "ns\t";
  double acc=ef*1E6*eQMratio/(RelGamma*RelGamma*RelGamma); 

  r_pos = r_pos + vel * dT*ns + 1/2*acc*(dT*ns)*(dT*ns);
  vel=vel+acc*dT*ns;
  RelBeta  = vel/c;
  RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);
  Et=RelGamma*E0; 
  cout << "Beta f:"<<RelBeta<<" \tPos:"<<r_pos<< "(m) \tT:"<< Et-E0<<"(MeV) \n";
}

}
