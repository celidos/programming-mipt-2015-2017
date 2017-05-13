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

    cout<< "after v1(" << v1.x() << "; " << v1.y() << " " << v1.z() << ") "
    << "v2(" << v2.x() << "; " << v2.y() << " " << v2.z() << ") "
    << "v3(" <<  v3.x() << "; " <<  v3.y() << " " <<  v3.z() << ") " <<  " |||| ";
    if ((v1 / v2 >= 0.0) && (v2 / v3 >= 0.0) && (v3 / v1 >= 0.0))
        return 1;
    else
        return 0;
}

vector3d interPointPlain(vector3d a, vector3d b, vector3d v1, vector3d v2, vector3d v3)
{

    vector3d dir = (b - a).norm();
    vector3d normal = ((v1 -v2)^(v1-v3)).norm();
    return a + dir * (volume(v1 - a, v2 - a, v3 - a) / ((v1 - v2) ^ (v1 - v3)).len()) /
            ( normal * dir );
}

DDOUBLE volume(vector3d a, vector3d b, vector3d c)
{
    return (a ^ b) * c;
}
