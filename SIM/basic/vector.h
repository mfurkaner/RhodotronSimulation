#include <iostream>
#include <iomanip>

#ifndef VECTOR_H
#define VECTOR_H

class vector3d{
private:
    double x;
    double y;
    double z;

public:
    vector3d(){x = 0; y= 0; z = 0;}
    vector3d(double x, double y, double z): x(x), y(y), z(z) {};

    double X(){return x;}
    double Y(){return y;}
    double Z(){return z;}
    void setX(double x) {this->x = x;}
    void setY(double y) {this->y = y;}
    void setZ(double z) {this->z = z;}

    vector3d operator+(vector3d);
    void operator+=(vector3d);
    vector3d operator-(vector3d);
    void operator-=(vector3d);
    vector3d operator*(double);  
    void operator*=(double);  
    vector3d operator/(double);  
    void operator/=(double);  
    bool operator==(vector3d);

    double operator*(vector3d);             // DOT PRODUCT
    vector3d operator%(vector3d);           // CROSS PRUDUCT

    double magnitude();
    vector3d direction();

    void rotate(vector3d around, double angle);   // angle is in degrees

    static vector3d from_string(std::string str);

};

std::istream& operator>>(std::istream &is, char const *pat);
std::ostream& operator<<(std::ostream& stream, vector3d& vect);  
std::ostream& operator<=(std::ostream& stream, vector3d& vect);  
std::istream& operator>>(std::istream& stream, vector3d& vect);

#endif