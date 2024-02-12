#include "fields.h"
#include <memory>

#pragma region RF_FIELD
/////////           RF FIELD
void RFField::update(double time){

}
/*
vector3d RFField::actOn(Electron& e){
    vector3d Efield = getEfield(e.pos);                            // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

vector3d RFField::actOnAndGetRungeKuttaCoef(Electron& e, double dt){
    // (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)/e_dummy.gamma()
    // is equal to
    // (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv()
    Electron e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    double dt_halved = dt/2;
    // get k1
    vector3d Efield = getEfield(e_dummy.pos);                                        // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                                             // Calculate F/m vector
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k2
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k1, dt_halved);
    Efield = getEfield(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;                                                      // Calculate F/m vector
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, dt_halved);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k3, dt);
    Efield = getEfield(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;   
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

    return (k1 + k2*2 + k3*2 + k4)/6;
}*/

#pragma endregion RF_FIELD


/////////           Coaxial RF FIELD
#pragma region COAXIAL_RF_FIELD

CoaxialRFField::~CoaxialRFField(){
    for(int i = 0; i < _childs.size(); i++){
        delete _childs[i];
    }
}
/*
vector3d CoaxialRFField::actOn(Electron& e){
    vector3d Efield = getEfield(e.pos);                            // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

vector3d CoaxialRFField::actOnAndGetRungeKuttaCoef(Electron& e, double dt){
    // (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)/e_dummy.gamma()
    // is equal to
    // (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv()
    Electron e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    double dt_halved = dt/2;
    // get k1
    vector3d Efield = getEfield(e_dummy.pos);                                        // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                                             // Calculate F/m vector
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k2
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k1, dt_halved);
    Efield = getEfield(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;                                                      // Calculate F/m vector
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, dt_halved);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt_halved);
    e_dummy.accelerate(k3, dt);
    Efield = getEfield(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;   
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

    return (k1 + k2*2 + k3*2 + k4)/6;
}*/

double CoaxialRFField::E_radial(double R)const{
    if (R == 0){ return 0;}
    double E_zero = E*E_max_pos/R;
    return E_zero*((R < -r1)*(R > -r2) | (R < r2)*(R > r1));
}

vector3d CoaxialRFField::getEfield(vector3d& position) const{
    double R = sqrt( position.X()*position.X() + position.Y()*position.Y() );
    double radial = E_radial( R );
    return position.direction() * radial * (position.Z() == 0.0 ? 1.0 : cos(2*M_PI * position.Z()/lambda));
}

double CoaxialRFField::getEfield( double R )const{
    return E_radial(R);
}

int CoaxialRFField::log( DataStorage& rf , double time, bool end){
    int count = 0;
    for( double i = -r2; i <= r2 ; i += 0.05){
        for (double j = -r2; j <= r2 ; j += 0.05){
            vector3d pos(i,j,0);
            vector3d Efield = getEfield(pos);
            if ( pos.magnitude() > r1 && Efield.magnitude() > 0){
                char temp[10];
                snprintf(temp, 10, "\n%.1lf", time);
                rf << temp << "," << pos << "," << Efield << "," << Efield.magnitude();
                count ++;
            }
        }
    }
    return count;
}

std::shared_ptr<CoaxialRFField> CoaxialRFField::Copy(){
    auto _copy = std::make_shared<CoaxialRFField>();
    _copy->E = E;
    _copy->r1 = r1;
    _copy->r2 = r2;
    _copy->E_max = E_max;
    _copy->E_max_pos = E_max_pos;
    _copy->frequency = frequency;
    _copy->phase_lag = phase_lag;
    return _copy;
}


void CoaxialRFField::split(uint32_t amount_of_child){
    _childs.clear();
    _childs.reserve(amount_of_child);
    for(uint32_t i = 0; i < amount_of_child; i++){
        auto child = new CoaxialRFField(phase_lag);
        
        child->setEmax(E_max);
        child->setR1(r1);
        child->setR2(r2);
        child->setEmaxPos(E_max_pos);
        child->setFreq(frequency);
        child->E = E;
        _childs.push_back(child);
    }
}

CoaxialRFField* CoaxialRFField::child(uint32_t index){
    return _childs[index];
}

void CoaxialRFField::update(double time){
    double w = 2 * M_PI * frequency * 0.001;
    E = sin(w*time + phase_lag*deg_to_rad)*E_max;
    
    for(uint32_t i = 0; i < _childs.size();i++){
        _childs[i]->update(time);
    }
}

#pragma endregion COAXIAL_RF_FIELD

#pragma region IMPORTED_RF_FIELD

vector3d ImportedRFField::getEfield(vector3d& position) const {
    RFFieldIndex nearestPosition = getNearestEPosition(position);
    try{

        return _fieldMaxValue.Efield.at(nearestPosition.z).field2D.at(nearestPosition.y).field1D.at(nearestPosition.x).E * _mult_time_coeff;
    }catch(std::out_of_range ex){
        return vector3d();
    }
}

RFFieldIndex ImportedRFField::getNearestEPosition(vector3d& position) const{
    RFFieldIndex res;
    double step = _fieldMaxValue.E_field_position_step;

    res.x = (std::round((position.X() - _fieldMaxValue.xmin)/step));
    res.y = (std::round((position.Y() - _fieldMaxValue.ymin)/step));
    res.z = (std::round((position.Z() - _fieldMaxValue.zmin)/step));

    return res;
}

vector3d ImportedRFField::getEmaxPosition() {return _fieldMaxValue.Emax_pos;}

void ImportedRFField::SetEmaxInLine(double theta, double newEmax){
    vector3d around(0,0,1);
    vector3d line(1,0,0);
    line.rotate(around, theta);

    double Emax_in_line = 0;

    for(double r = _fieldMaxValue.xmin; r <= _fieldMaxValue.xmax;){
        vector3d pos = line * r;

        double E = getEfield(pos).magnitude();
        if(E > Emax_in_line){
            Emax_in_line = E;
        }

        r += _fieldMaxValue.E_field_position_step;
    }
    double mult_const = newEmax/Emax_in_line;
    for(int z_i = 0 ; z_i < _fieldMaxValue.Efield.size(); z_i++){
        for(int y_i = 0 ; y_i < _fieldMaxValue.Efield[z_i].field2D.size(); y_i++){
            for(int x_i = 0 ; x_i < _fieldMaxValue.Efield[z_i].field2D[y_i].field1D.size(); x_i++){
                _fieldMaxValue.Efield[z_i].field2D[y_i].field1D[x_i].E *= mult_const;
            }
        }
    }

    std::cout << "mult_const: " << mult_const << std::endl;

}

void ImportedRFField::Multiply(double mult_const){
    for(int z_i = 0 ; z_i < _fieldMaxValue.Efield.size(); z_i++){
        for(int y_i = 0 ; y_i < _fieldMaxValue.Efield[z_i].field2D.size(); y_i++){
            for(int x_i = 0 ; x_i < _fieldMaxValue.Efield[z_i].field2D[y_i].field1D.size(); x_i++){
                _fieldMaxValue.Efield[z_i].field2D[y_i].field1D[x_i].E *= mult_const;
            }
        }
    }
}

void SetMinMax(double val, double& min, double& max){
    if( val < min ){
        min = val;
    }
    if( val > max ){
        max = val;
    }
}

void ImportedRFField::Import(std::string filepath){
        std::ifstream fin; 
        fin.open(filepath, std::ios::in);
        std::string line;
        
        double x, y, z, Ex, Ey, Ez, xmin, xmax, ymin, ymax, zmin, zmax, Emax;

        do{
            std::getline(fin, line);
        }while(!fin.eof() && line.find("#", 0) != std::string::npos);
        sscanf(line.c_str(), "%lf %lf %lf %lf %lf %lf", &x, &y, &z, &Ex, &Ey, &Ez);

        Ex *= 0.000001;
        Ey *= 0.000001;
        Ez *= 0.000001;

        xmin = x;
        xmax = x;
        ymin = y;
        ymax = y;
        zmax = z;
        zmin = z;

        Emax = sqrt(Ex*Ex + Ey*Ey + Ez*Ez);

        do{
            double z_child;
            z_child = z;
            RFFieldOnConstZPlane rfplane;
            rfplane.z = z;

            while(z == z_child && !fin.eof()){
                double y_child = y;
                RFFieldOnConstZYLine rfline;
                rfline.y = y;
                while(y == y_child && z == z_child && !fin.eof()){
                    RFFieldOnConstXYZPoint rfpoint;
                    rfpoint.x = x;
                    rfpoint.E = vector3d(Ex, Ey, Ez);

                    rfline.field1D.push_back(rfpoint);

                    std::getline(fin, line);
                    if ( line.find("#", 0) != std::string::npos){
                        continue;
                    }
                    sscanf(line.c_str(), "%lf %lf %lf %lf %lf %lf", &x, &y_child, &z_child, &Ex, &Ey, &Ez);
                    Ex *= 0.000001;
                    Ey *= 0.000001;
                    Ez *= 0.000001;
                    SetMinMax(x, xmin, xmax);
                    SetMinMax(y_child, ymin, ymax);
                    SetMinMax(z_child, zmin, zmax);
                    double E = sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
                    if(z_child == 0 && Emax < E){
                        Emax = E;
                        _fieldMaxValue.Emax_pos = vector3d(x, y_child, z_child);
                    }

                    // Edge case, we read the last line , z and y is same but x is different
                    if(fin.eof() && z == z_child && y == y_child){
                        RFFieldOnConstXYZPoint rfpoint_end;
                        rfpoint_end.x = x;
                        rfpoint_end.E = vector3d(Ex, Ey, Ez);
                        rfline.field1D.push_back(rfpoint_end);
                    }
                }
                rfplane.field2D.push_back(rfline);
                if(y != y_child){
                    _fieldMaxValue.E_field_position_step = abs(y - y_child);
                }

                // Edge case, we read the last line, z is same but y is different
                if(fin.eof() && z == z_child && y != y_child){
                    RFFieldOnConstZYLine rfline_end;
                    RFFieldOnConstXYZPoint rfpoint_end;
                    rfpoint_end.x = x;
                    rfpoint_end.E = vector3d(Ex, Ey, Ez);
                    rfline_end.y = y_child;
                    rfline_end.field1D.push_back(rfpoint_end);
                    rfplane.field2D.push_back(rfline_end);
                }
                y = y_child;
            }
            _fieldMaxValue.Efield.push_back(rfplane);
            
            // Edge case, we read the last line, z is different
            if(fin.eof() && z != z_child ){
                RFFieldOnConstZPlane  rfplane_end;
                RFFieldOnConstZYLine rfline_end;
                RFFieldOnConstXYZPoint rfpoint_end;

                rfpoint_end.x = x;
                rfpoint_end.E = vector3d(Ex, Ey, Ez);
                rfline_end.y = y;
                rfline_end.field1D.push_back(rfpoint_end);
                rfplane_end.z = z_child;
                rfplane_end.field2D.push_back(rfline_end);
                _fieldMaxValue.Efield.push_back(rfplane_end);       
            }
            z = z_child;
        }while( !fin.eof() );

        _fieldMaxValue.xmax = xmax;
        _fieldMaxValue.ymax = ymax;
        _fieldMaxValue.zmax = zmax;
        _fieldMaxValue.xmin = xmin;
        _fieldMaxValue.ymin = ymin;
        _fieldMaxValue.zmin = zmin;

        fin.close();
}


void ImportedRFField::update(double time){
    double w = 2 * M_PI * frequency * 0.001;
    _mult_time_coeff = sin(w*time + phase_lag*deg_to_rad);
}

#pragma endregion IMPORTED_RF_FIELD



/////////           Magnet
/*
double Magnet::getOptimalB(double E, double minB, double maxB, double stepsize){
    Electron e;
    e.Et = E + E0;
    double mag = e.get_vel();
    double min_r = 0.752967;
    double optB = 0;

    for(double B = minB; B < maxB ; B += stepsize){
        e.pos = vector3d(0.188241,0,0);
        e.vel = vector3d(mag, 0, 0);
        MagneticField mf;
        mf.addMagnet(B, this->r, this->position);
        for(double t = 0; t < 10 ; t+=0.001){
            vector3d acc_B = mf.actOn(e);
            e.accelerate(acc_B , 0.001);
            e.move(acc_B, 0.001);
            if(e.pos.magnitude() < min_r){
                min_r = e.pos.magnitude();
                optB = B;
            }
        }
    }
    //cout << min_r << "  ";
    return optB;
}*/

/////////           MAGNETIC FIELD
MagneticField::~MagneticField(){
    for(int i = 0; i < _childs.size(); i++){
        delete _childs[i];
    }
}

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position){
    vector3d relative = e_position - hs_position;                                       // get the relative position of electron with respect to magnet center
    if ( relative.magnitude() <= r && relative * hs_position.direction() >= -r/5){          // if the relative distance is less than r && relative doesn't have opposite direction
        return true;                                                                    // electron is inside the half sphere
    }
    return false;
}

int MagneticField::isInside(vector3d position) const{
    for(int i = 0; i < magnets.size() ; i++){
        if ( isInsideHalfSphere(position, magnets[i].r, magnets[i].position ) ){
            return i;
        }
    }
    return -1;
}

int MagneticField::isInsideDebug(){
    return magnets.size() == 0 ? -1 : 0;
}

void MagneticField::addMagnet(double B, double r, vector3d position){
    magnets.push_back(Magnet(B, r, position));
    //hasEntered.push_back(false);
}

void MagneticField::addMagnet(Magnet m){
    magnets.push_back(m);
    //hasEntered.push_back(false);
}

vector3d MagneticField::getField(vector3d position)const{
    int magnet_index = isInside(position);
    if ( magnet_index == -1 ){
        return vector3d(0, 0, 0);
    }
    if (false && !hasEntered[magnet_index] ){ 
        //hasEntered[magnet_index] = true; 
        //vector3d v = position - magnets[magnet_index].position;
        //relativeEnterDistance.push_back(v.magnitude());
    }
    return vector3d(0, 0, magnets[magnet_index].B);
}
/*
vector3d MagneticField::actOn(Electron& e){
    if (isInside(e.pos) == -1){
        return vector3d(0,0,0);
    }

    vector3d Bfield = getField(e.pos);                                          // Calculate B vector
    vector3d F_m = (e.vel % Bfield)*eQMratio;                                   // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();                   // Calculate a vector
    return acc;
}

vector3d MagneticField::actOnAndGetRungeKuttaCoef(Electron& e, double dt){
    if (isInside(e.pos) == -1){
        return vector3d(0,0,0);
    }
    Electron e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    double time_halved = dt/2;
    // get k1
    vector3d Bfield = getField(e.pos);                                             
    vector3d F_m = (e_dummy.vel % Bfield)*eQMratio;                                       
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();     
    // get k2
    e_dummy.move(time_halved);
    e_dummy.accelerate(k1, time_halved);
    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;                                               
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();    
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, time_halved);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();   
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(time_halved);
    e_dummy.accelerate(k3, dt);
    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;  
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

    return (k1 + k2*2 + k3*2 + k4)/6;
}*/


vector3d MagneticField::getJerk(vector3d pos, vector3d vel, vector3d acc){
    if(acc == vector3d(0,0,0)){
        return vector3d(0,0,0);
    }
    vector3d Bfield = getField(pos);   
    vector3d F_m = (vel % Bfield)*eQMratio;        
    vector3d derF_m = (acc % Bfield)*eQMratio;
    double A = acc * vel / (c*c);
    A *= -2*(1 - vel*vel/(c*c));
    return (vel*(acc * F_m)/(c*c) + vel*(vel*derF_m)/(c*c) + acc*(vel*F_m)/(c*c) - derF_m)*A;
}

void MagneticField::log(DataStorage& magnet){
    for(double x = -2; x <= 2 ; x+=0.001){
        for(double y = -2; y <= 2; y+=0.001){
            vector3d v(x,y,0);
            if ( getField(v).Z() != 0 ){
                magnet << x << " " << y << "\n";
            }
        }
    }
}


std::shared_ptr<MagneticField> MagneticField::LightWeightCopy(){
    auto newMagneticField = std::make_shared<MagneticField>();
    newMagneticField->magnets = magnets;
    return newMagneticField;
}

void MagneticField::split(uint32_t amount_of_child){
    _childs.clear();
    _childs.reserve(amount_of_child);
    for(uint32_t i = 0; i < amount_of_child; i++){
        _childs.push_back(new MagneticField());
        _childs[i]->magnets = magnets;
    }
}

MagneticField* MagneticField::child(uint32_t index){
    return _childs[index];
}


double rho_to_magnetic_field(double rho, double Ek){
    double B = c/(eQMratio*rho);
    double gamma = (Ek + E0)/E0;
    B *= gamma*sqrt(1- 1/(gamma*gamma));
    return B;
}

double magnetic_field_to_rho(double B, double Ek){
    double rho = c/(eQMratio*B);
    double gamma = (Ek + E0)/E0;
    rho *= gamma*sqrt(1- 1/(gamma*gamma));
    return rho;
}