#include "rt_geometry.h"

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

    if ((v1 / v2 >= ZERO_LIMIT) && (v2 / v3 >= ZERO_LIMIT) && (v3 / v1 >= ZERO_LIMIT))
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

float cosangle2v(vector3d a, vector3d b)
{
    return (a * b) / (a.len() * b.len());
}

DDOUBLE angle2v(vector3d a, vector3d b)
{
    return acos((a * b) / (a.len() * b.len()));
}

int isOnTheOneSide(vector3d p, vector3d n, vector3d a, vector3d b)
{
    DDOUBLE x = (a - p) * n,
            y = (b - p) * n;
    if (fabs(x) <= ZERO_LIMIT || fabs(y) <= ZERO_LIMIT)
        return 0;
    if (x * y > 0.0)
        return 1;
    return -1;
}

vector3d reflectVector(vector3d a, vector3d n)
{
    return n * (a * n) * 2.0 - a;
}

/*vector3d refractVector(vector3d a, vector3d n, float k) {

    float angle = angle2v(a, n);
    std::cout << "refractin a" << a.toStr() << " over n" <<n.toStr() << ", with k = " << k << "; angle = " << angle;
    angle = asin(k*sin(angle)) - angle;
    std::cout << "new angle = " << angle << std::endl;
    vector3d axis = (a ^ n).norm();
    float cosa = cos(angle);
    float sina = sin(angle);
    DDOUBLE &x = axis[0];
    DDOUBLE &y = axis[1];
    DDOUBLE &z = axis[2];

    float M[3][3] = {{(1-cosa)*x*x + cosa,   (1-cosa)*x*y - sina*z, (1-cosa)*x*z + sina*y},
                     {(1-cosa)*y*x + sina*z, (1-cosa)*y*y + cosa,   (1-cosa)*y*z - sina*x},
                     {(1-cosa)*z*x - sina*y, (1-cosa)*z*y + sina*x, (1-cosa)*z*z + cosa  }};

    return vector3d(a[0] * M[0][0] + a[1] * M[0][1] + a[2] * M[0][2],
                    a[0] * M[1][0] + a[1] * M[1][1] + a[2] * M[1][2],
                    a[0] * M[2][0] + a[1] * M[2][1] + a[2] * M[2][2]);
}*/

vector3d refractVector(vector3d A, vector3d N, float K, bool &isOk)
{
    vector3d a = A;
    vector3d n = N;
    float k = K;

    float eta = 1/k;
    float cos_theta = -cosangle2v(a, n);//(n * a) / a.len();
    std::cout << "refract a" << a.toStr() << ", n" << n.toStr() << "; k = " << k << "; costheta = " << cos_theta;
    if (cos_theta < -ZERO_LIMIT) {
        cos_theta *= -1.0;
        n = -n;
        eta = std::max(float(k), float(1.0 / k));
    } else {
        eta = std::min(float(k), float(1.0 / k));
    }
    float m = 1.0f - eta * eta * (1.0 - cos_theta * cos_theta);
    std::cout << ", m = " << m ;
    vector3d bb;
    if (m >= 0.0)
        bb = (a*eta + n*(eta * cos_theta - sqrt(m))).norm();
    else
        bb = reflectVector(A, N);
    std::cout << ", a" << a.toStr()<<"; isok = "<< (m >0.0) << std::endl;
    isOk = true;
    return bb;
}

DDOUBLE distPointPlain(vector3d p, vector3d v1, vector3d v2, vector3d v3)
{
    return volume(v1 - p, v2 - p, v3 - p) / ((v1 - v2) ^ (v1 - v3)).len();
}



