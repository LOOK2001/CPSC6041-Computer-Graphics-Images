#ifndef WARPER_HELPER_H
#define WARPER_HELPER_H

#include <iostream>

#include "matrix.h"

using namespace std;
using std::string;


/*
Multiply M by a rotation matrix of angle theta
*/
void Rotate(Matrix3D &M, float theta){
    Matrix3D R; // this initializes R to identity
    double rad = PI * theta / 180.0; // convert degrees to radians

    // populate the rotation matrix
    R[0][0] = cos(rad);
    R[0][1] = -sin(rad);
    R[1][0] = sin(rad);
    R[1][1] = cos(rad);

    // append the rotation to transformation Matrix
    M = R * M;
}

/*
Multiply M by a scaling matrix of value sx, sy
*/
void Scale(Matrix3D &M, float sx, float sy)
{
    Matrix3D R;
    R[0][0] = sx;
    R[1][1] = sy;

    M = R * M;
}

/*
Multiply M by a translation matrix of value dx, dy
*/
void translate(Matrix3D &M, float dx, float dy)
{
    Matrix3D R;
    R[0][2] = dx;
    R[1][2] = dy;

    M = R * M;
}

/*
Multiply M by a flip matrix of value xf, yf
*/
void flip(Matrix3D &M, float xf, float yf)
{
    Matrix3D R;
    if (xf == 1) R[0][0] = -1;
    if (yf == 1) R[1][1] = -1;

    M = R * M;
}

/*
Multiply M by a shear matrix of value hx, hy
*/
void shear(Matrix3D &M, float hx, float hy)
{
    Matrix3D R;
    R[0][1] = hx;
    R[1][0] = hy;

    M = R * M;
}

/*
Multiply M by a perspective matrix of value px, py
*/
void perspective(Matrix3D &M, float px, float py)
{
    Matrix3D R;
    R[2][0] = px;
    R[2][1] = py;

    M = R * M;
}

#endif // IMAGE_OPERWARPER_HELPER_HATOR_H
