#ifndef CONSTS_H
    #include "consts.h"
#endif

#ifndef VECTOR_H
    #include "vector.h"
#endif

#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif


using namespace std;
#define RHODO2D_H

double Eradial(double r, double time, double phase);


typedef pair<double, double> giris_cikis_tpair;

class Electron2D{
public:
    double Et = E0 + Ein/1000;
    vector3d pos;
    vector3d vel;
    double RFphase = 0;
    vector< double > enerjiler;
    vector< giris_cikis_tpair > t_giris_cikis;
    bool isinside = true;
    Electron2D(){
        t_giris_cikis.push_back(giris_cikis_tpair(0,0));
        pos.setX(-R2);
        vel.setX(get_vel());
    }
    double get_vel();
    double get_travel_time(double dist);

    void setEin(double E_in){this->Et = E_in + E0; vel.setX(get_vel());};

    void print_electron_info();
    void move(double dt);
    void move(vector3d acc, double dt);
    void accelerate(vector3d acc, double dt);
    void e_gecis(double &t);
    double beta(){
        return vel.magnitude()/c;
    }
    double gamma(){
        return 1/(sqrt(1 - beta()*beta()));
    }
};



typedef pair<double, double> max_energy_rms_pair;

class Bunch2D{
private:
    int e_count = NUM_OF_ELECTRONS;
    double initial_length_ns = GUN_ACTIVE_TIME;
    double ns_between = GUN_ACTIVE_TIME/(NUM_OF_ELECTRONS - 1);
    int index_fastest = 0;
    double max_energy = 0;
    double entry_time = 0;
    double E_in = 0.04;
public:
    vector<Bunch2D> subBunchs;
    vector<Electron2D> e;
    Bunch2D(unsigned int num_of_electrons){
        e_count = num_of_electrons;
        for(int i = 0 ; i < num_of_electrons ; i++){
            e.push_back(Electron2D());
        }        
    }
    Bunch2D(){
        Bunch2D(1);
    }

    Electron2D& getFastest();
    void setEin(double E_in){ this->E_in = E_in ; for (int i = 0; i < e.size() ; i++){ e[i].setEin(E_in);}}
    double getEin(){ return E_in;}
    void interact(RFField& E, MagneticField& B, double time, double time_interval, DataStorage& ds);
    void divide(unsigned int num);
    Bunch2D& subBunch(unsigned int index);
    vector<Bunch2D*> subBunchPtr();
    double E_ave();
    double E_rms();
    void reset();
    void print_bunch_info();
    void print_summary();
};