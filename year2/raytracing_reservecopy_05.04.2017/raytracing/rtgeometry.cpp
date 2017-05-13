#include "rtgeometry.h"

#include <iostream>

using std::cout;
using std::endl;

DDOUBLE dist(vector3d a, vector3d b) {
    return (b - a).len();
}

DDOUBLE dist2(vector3d a, vector3d b) {
    return (b - a).len2();
}

DDOUBLE distPointLine(vector3d p, vector3d a, vector3d b) {
    return ((p - a) ^ (p - b)).len() / ((b - a).len());
}

vector3d projPointLine(vector3d p, vector3d a, vector3d b) {
    vector3d dir = (b - a).norm();
    return a + dir * (dir * (p - a));
}

int isPointOnTriangle(vector3d p, vector3d a, vector3d b, vector3d c)
{
    vector3d v1 = ((b - a) ^ (p - a)),
             v2 = ((c - b) ^ (p - b)),
             v3 = ((a - c) ^ (p - c));

//    cout<< "after v1(" << v1.x() << "; " << v1.y() << " " << v1.z() << ") "
//    << "v2(" << v2.x() << "; " << v2.y() << " " << v2.z() << ") "
//    << "v3(" <<  v3.x() << "; " <<  v3.y() << " " <<  v3.z() << ") " <<  " |||| ";
    if ((v1 / v2 >= 0.0) && (v2 / v3 >= 0.0) && (v3 / v1 >= 0.0))
        return 1;
    else
        return 0;
}

vector3d interLinePlain(vector3d a, vector3d b, vector3d v1, vector3d v2, vector3d v3)
{
    vector3d dir = (b - a).norm();
    vector3d normal = ((v1 -v2)^(v1-v3)).norm();
    vector3d scaleddir = dir * distPointPlain(a, v1, v2, v3) / (normal * dir);
    if (fabs(distPointPlain(a + scaleddir, v1, v2, v3)) < ZERO_LIMIT)
        return a + scaleddir;
    return a - scaleddir;
}

DDOUBLE volume(vector3d a, vector3d b, vector3d c)
{
    return (a ^ b) * c;
}

DDOUBLE cosangle(vector3d a, vector3d b)
{
    return (a * b) / (a.len() * b.len());
}

int isOnTheOneSide(vector3d p, vector3d n, vector3d a, vector3d b)
{
    DDOUBLE x = (a - p) * n,
            y = (b - p) * n;
    if (fabs(x) <= ZERO_LIMIT || fabs(y) <+ ZERO_LIMIT)
        return 0;
    if (x * y > 0.0)
        return 1;
    return -1;
}

vector3d reflectVector(vector3d a, vector3d n)
{
    return n * (a * n) * 2.0 - a;
}

DDOUBLE distPointPlain(vector3d p, vector3d v1, vector3d v2, vector3d v3)
{
    return volume(v1 - p, v2 - p, v3 - p) / ((v1 - v2) ^ (v1 - v3)).len();
}



