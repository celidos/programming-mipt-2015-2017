#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>

typedef double DDOUBLE;

const DDOUBLE ZERO_LIMIT = 1e-10;

inline DDOUBLE sq(const DDOUBLE x) {
    return x * x;
}

class vector3d
{
public:
    DDOUBLE v[3];

    vector3d() {
        v[0] = v[1] = v[2] = 0.0;
    }

    vector3d(DDOUBLE coord[3]) {
        v[0] = coord[0];
        v[1] = coord[1];
        v[2] = coord[2];
    }

    vector3d(DDOUBLE x, DDOUBLE y, DDOUBLE z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    DDOUBLE x() const {
        return v[0];
    }

    DDOUBLE y() const {
        return v[1];
    }

    DDOUBLE z() const {
        return v[2];
    }

    DDOUBLE & operator[](int i) {
        return v[i];
    }

    DDOUBLE len() const {
        return sqrt(sq(v[0]) + sq(v[1]) + sq(v[2]));
    }

    DDOUBLE len2() const {
        return sq(v[0]) + sq(v[1]) + sq(v[2]);
    }

    bool iszero() const {
        return sq(v[0]) + sq(v[1]) + sq(v[2]) < ZERO_LIMIT;
    }

    vector3d norm() {
        DDOUBLE len = this->len();
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
        return *this;
    }

    vector3d operator+() {
        return *this;
    }

    vector3d operator-() {
        return vector3d(-v[0], -v[1], -v[2]);
    }

    vector3d operator+(const vector3d other) {
        return vector3d(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
    }

    vector3d operator-(const vector3d other) {
        return vector3d(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
    }

    // dot (scalar) product
    DDOUBLE operator*(const vector3d other) {
        return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
    }

    // cross product
    vector3d operator^(const vector3d other) {
        return vector3d(v[1]*other.v[2] - v[2]*other.v[1],
                        v[2]*other.v[0] - v[0]*other.v[2],
                        v[0]*other.v[1] - v[1]*other.v[0]);
    }

    vector3d operator*(const DDOUBLE scale) {
        return vector3d(v[0] * scale, v[1] * scale, v[2] * scale);
    }

    vector3d operator/(const DDOUBLE scale) {
        return vector3d(v[0] / scale, v[1] / scale, v[2] / scale);
    }

    vector3d operator*(const int scale) {
        return vector3d(v[0] * scale, v[1] * scale, v[2] * scale);
    }

    vector3d operator/(const int scale) {
        return vector3d(v[0] / scale, v[1] / scale, v[2] / scale);
    }

    vector3d operator=(const vector3d other) {
        this->v[0] = other.v[0];
        this->v[1] = other.v[1];
        this->v[2] = other.v[2];
        return *this;
    }

    DDOUBLE dist(vector3d a) {
        return (*this - a).len();
    }

    DDOUBLE dist2(vector3d a) {
        return (*this - a).len2();
    }

    // ratio of two PARALLEL vectors
    DDOUBLE operator / (const vector3d other) {
        if (fabs(v[0]) > ZERO_LIMIT && fabs(other.v[0]) > ZERO_LIMIT)
            return v[0] / other.v[0];
        else if (fabs(v[1]) > ZERO_LIMIT && fabs(other.v[1]) > ZERO_LIMIT)
            return v[1] / other.v[1];
        else
            return v[2] / other.v[2];
    }
};

DDOUBLE dist (vector3d a, vector3d b);
DDOUBLE dist2(vector3d a, vector3d b);
DDOUBLE volume(vector3d a, vector3d b, vector3d c);
DDOUBLE distPointLine (vector3d p, vector3d a, vector3d b);
vector3d projPointLine(vector3d p, vector3d a, vector3d b);
vector3d interPointPlain(vector3d a, vector3d b, vector3d v1, vector3d v2, vector3d v3);
int isPointOnTriangle(vector3d p, vector3d a, vector3d b, vector3d c);

#endif // GEOMETRY_H
