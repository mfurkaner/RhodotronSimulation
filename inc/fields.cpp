#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif

/////////           RF FIELD
void RFField::update(double time){
    double w = 2 * M_PI * frequency / 1000;
    E = sin(w*time + phase_lag*deg_to_rad)*E_max;
}

vector3d RFField::actOn(Electron2D& e){
    vector3d Efield = getField(e.pos);                            // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

vector3d RFField::actOnAndGetRungeKuttaCoef(Electron2D& e, double dt){
    Electron2D e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    // get k1
    vector3d Efield = getField(e_dummy.pos);                                        // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                                             // Calculate F/m vector
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k2
    e_dummy.move(dt/2);
    e_dummy.accelerate(k1, dt/2);
    Efield = getField(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;                                                      // Calculate F/m vector
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, dt/2);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt/2);
    e_dummy.accelerate(k3, dt);
    Efield = getField(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;   
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();

    return (k1 + k2*2 + k3*2 + k4)/6;
}

/////////           Coaxial RF FIELD
vector3d CoaxialRFField::actOn(Electron2D& e){
    vector3d Efield = getField(e.pos);                            // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

vector3d CoaxialRFField::actOnAndGetRungeKuttaCoef(Electron2D& e, double dt){
    Electron2D e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    // get k1
    vector3d Efield = getField(e_dummy.pos);                                        // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                                             // Calculate F/m vector
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k2
    e_dummy.move(dt/2);
    e_dummy.accelerate(k1, dt/2);
    Efield = getField(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;                                                      // Calculate F/m vector
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, dt/2);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      // Calculate a vector
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt/2);
    e_dummy.accelerate(k3, dt);
    Efield = getField(e_dummy.pos);                                                 // Calculate E vector
    F_m = Efield*1E6*eQMratio;   
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();

    return (k1 + k2*2 + k3*2 + k4)/6;
}

double CoaxialRFField::E_radial(double R){
    double E_zero = E*E_max_pos/R;
    double res = E_zero*((R < -r1)*(R > -r2) | (R < r2)*(R > r1));
    return res;
}

vector3d CoaxialRFField::getField(vector3d position){
    double radial = E_radial( position.magnitude() );
    return position.direction() * radial;
}

double CoaxialRFField::getField( double R ){
    return E_radial(R);
}

int CoaxialRFField::log( DataStorage& rf , double time){
    int count = 0;
    for( double i = -r2; i <= r2 ; i += 0.05){
        for (double j = -r2; j <= r2 ; j += 0.05){
            vector3d pos(i,j,0);
            vector3d Efield = getField(pos);
            if ( pos.magnitude() > r1 ){
                rf << "t: " << time << " p: " << pos << "  E: " << Efield << "   mag: " << Efield.magnitude() <<"\n";
                count ++;
            }
        }
    }
    return count;
}

/////////           Magnet
double Magnet::getOptimalB(double E, double minB, double maxB, double stepsize){
    Electron2D e;
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
    cout << min_r << "  ";
    return optB;
}

/////////           MAGNETIC FIELD
bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position){
    vector3d relative = e_position - hs_position;                                       // get the relative position of electron with respect to magnet center
    if ( relative.magnitude() <= r && relative * hs_position.direction() >= -r/5){          // if the relative distance is less than r && relative doesn't have opposite direction
        return true;                                                                    // electron is inside the half sphere
    }
    return false;
}

int MagneticField::isInside(vector3d position){
    for(int i = 0; i < magnets.size() ; i++){
        if ( isInsideHalfSphere(position, magnets[i].r, magnets[i].position ) ){
            return i;
        }
    }
    return -1;
}

int MagneticField::isInsideDebug(){
    return 0;
}

void MagneticField::addMagnet(double B, double r, vector3d position){
    magnets.push_back(Magnet(B, r, position));
    hasEntered.push_back(false);
}

void MagneticField::addMagnet(Magnet m){
    magnets.push_back(m);
    hasEntered.push_back(false);
}

vector3d MagneticField::getField(vector3d position){
    int magnet_index = isInsideDebug();
    if ( magnet_index == -1 ){
        return vector3d(0, 0, 0);
    }
    if ( !hasEntered[magnet_index] ){ 
        hasEntered[magnet_index] = true; 
        vector3d v = position - magnets[magnet_index].position;
        relativeEnterDistance.push_back(v.magnitude());
    }
    return vector3d(0, 0, magnets[magnet_index].B);
}

vector3d MagneticField::actOn(Electron2D& e){
    if (isInsideDebug() == -1){
        return vector3d(0,0,0);
    }

    vector3d Bfield = getField(e.pos);                                          // Calculate B vector
    vector3d F_m = (e.vel % Bfield)*eQMratio;                                   // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();                   // Calculate a vector
    return acc;
}

vector3d MagneticField::actOnAndGetRungeKuttaCoef(Electron2D& e, double dt){
    if (isInsideDebug() == -1){
        return vector3d(0,0,0);
    }
    Electron2D e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    // get k1
    vector3d Bfield = getField(e.pos);                                             
    vector3d F_m = (e_dummy.vel % Bfield)*eQMratio;                                       
    vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();     
    // get k2
    e_dummy.move(dt/2);
    e_dummy.accelerate(k1, dt/2);
    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;                                               
    vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();      
    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(k2, dt/2);
    vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();     
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt/2);
    e_dummy.accelerate(k3, dt);
    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;  
    vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)/(c*c))/e_dummy.gamma();

    return (k1 + k2*2 + k3*2 + k4)/6;
}


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
                magnet << x << "  " << y << "\n";
            }
        }
    }
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