#include "bunch.h"


void Bunch::reset(){
    for(int i = 0; i < e_count ; i++){
        e[i]->pos = _LEGACY_gun_pos;
        e[i]->Et = E0 + E_in;
        e[i]->vel = vector3d(e[i]->get_vel(), 0, 0);
    }
}

double Bunch::E_ave(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += e[i]->Et - E0;
  }
  return result/e_count;
}

double Bunch::E_rms(){
  double result = 0;
  for(int i = 0; i < e_count ; i++){
    result += (e[i]->Et - E0 - E_ave()) * (e[i]->Et - E0 - E_ave());
  }
  return sqrt(result/e_count);
}


void Bunch::interact(RFField& E, MagneticField& B, const double time, double time_interval){
    for(int i = 0; i < e.size() ; i++){

        //#ifdef LEAP_FROG
        Interactor::q_EM_interaction_LF(*e[i], E, B, time_interval);
        //#endif

        #ifdef RUNGE_KUTTA
        Interactor::q_EM_interaction_RK(*e[i], E, B, time_interval);
        #endif

    }
}
/*
void Bunch::concat(){
    while( !e.empty() ) e.pop_back();
    for(int i = 0; i < subBunchs.size(); i++){
        for(int j = 0; j < subBunchs.at(i).e.size() ; j++){
            e.push_back(subBunchs.at(i).e.at(j));
        }
    }
}

Bunch& Bunch::subBunch(unsigned int index){
    if(subBunchs.size() > index){
        return subBunchs.at(index);
    }
    return *this;
}

vector<Bunch*> Bunch::subBunchPtr(){
    vector<Bunch*> ptrv;
    for(int i = 0; i < subBunchs.size() ; i++ ){
        ptrv.push_back(&subBunchs[i]);
    }
    return ptrv;
}*/

void Bunch::print_summary(){
  //cout << "Electron with the most energy : " << index_fastest + 1 << ") " << e[index_fastest]->Et - E0 << " MeV,\tE_ave of bunch : "<< E_ave() << " MeV,\tRMS of bunch : " << E_rms() <<  " MeV" << endl;
}

void Bunch::print_bunch_info(){
    //cout<<std::setprecision(4);
    for(int i = 0; i < e_count;i++){
        if( i == index_fastest ){
        //cout << "** ";
        }
        //cout << "Electron " << i+1 << ":" << endl;
        e[i]->PrintInfo();
    }
    print_summary();
}