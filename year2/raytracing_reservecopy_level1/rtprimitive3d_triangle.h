#ifndef PRIMITIVE3D_TRIANGLE_H
#define PRIMITIVE3D_TRIANGLE_H

#include "rtprimitive3d.h"

class Triangle3d : public Primitive3d
{
public:
    vector3d vx[3];
    Triangle3d(vector3d A, vector3d B, vector3d C, Color color, bool inOrderOrient = true) {
        vx[0] = A;
        vx[1] = B;
        vx[2] = C;
        orient = inOrderOrient;
        this->color = color;
        type = PT_TRIANGLE;
    }

    Triangle3d(vector3d vertexes[3], Color color, bool inOrderOrient = true) {
        vx[0] = vertexes[0];
        vx[1] = vertexes[1];
        vx[2] = vertexes[2];
        orient = inOrderOrient;
        this->color = color;
        type = PT_TRIANGLE;
    }

    vector3d normal() {
        return ((vx[2] - vx[0]) ^ (vx[1] - vx[0])).norm();
    }

    bool operator||(vector3d direction) {
        return fabs(normal() * direction) < ZERO_LIMIT;
    }

    int  orientate(vector3d normal) { };
};

#endif // PRIMITIVE3D_TRIANGLE_H
