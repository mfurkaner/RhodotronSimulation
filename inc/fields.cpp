#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif

void RFField::update(double time){
    double w = 2 * M_PI * frequency / 1000;
    E = sin(w*time + phase_lag*deg_to_rad)*Emax;
}

double RFField::E_radial(double R){
    double E_zero = E*Emax_pos/R;
    double res = E_zero*((R < -R1*1000)*(R > -R2*1000) | (R < R2*1000)*(R > R1*1000));
    return res;
}

vector3d RFField::getField(vector3d position){
    double radial = E_radial( position.magnitude()*1000 );
    return position.direction() * radial;
}

double RFField::getField( double R ){
    return E_radial(R*1000);
}

vector3d RFField::actOn(Electron2D& e){
    double vel = e.get_vel();
    double RelBeta = vel/c;
    double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

    vector3d Efield = getField(e.pos);                            // Calculate E vector
    vector3d acc = Efield*1E6*eQMratio/(RelGamma*RelGamma*RelGamma);    // Calculate a vector
    return acc;
}

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position){
    vector3d relative = e_position - hs_position;                                       // get the relative position of electron with respect to magnet center
    if ( relative.magnitude() <= r && relative * hs_position.direction() >= 0 ){        // if the relative distance is less than r && relative doesn't have opposite direction
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

void MagneticField::addMagnet(double B, double r, vector3d position){
    magnets.push_back(Magnet(B, r, position));
}

vector3d MagneticField::getField(vector3d position){
    int magnet_index = isInside(position);
    if ( magnet_index == -1 ){
        return vector3d(0, 0, 0);
    }
    return vector3d(0, 0, magnets[magnet_index].B);
}

vector3d MagneticField::actOn(Electron2D& e){
    if (isInside(e.pos) == -1){
        return vector3d(0,0,0);
    }
    double vel = e.get_vel();
    double RelBeta = vel/c;
    double RelGamma = 1.0 / sqrt(1.0-RelBeta*RelBeta);

    vector3d Bfield = getField(e.pos);                                          // Calculate B vector
    vector3d acc = (e.vel % Bfield)*eQMratio/(RelGamma*RelGamma*RelGamma);  // Calculate a vector
    return acc;
}