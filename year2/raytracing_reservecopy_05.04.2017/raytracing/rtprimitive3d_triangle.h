#ifndef PRIMITIVE3D_TRIANGLE_H
#define PRIMITIVE3D_TRIANGLE_H

#include <vector>

#include "rtprimitive3d.h"

class Triangle3d : public Primitive3d
{
public:
    std::vector<vector3d> vx;
    Triangle3d(vector3d A, vector3d B, vector3d C, Color color, bool inOrderOrient = true) {
        vx.push_back(A);
        vx.push_back(B);
        vx.push_back(C);
        orient = inOrderOrient;
        this->color = color;
        type = PT_TRIANGLE;
    }

    Triangle3d(vector3d vertexes[3], Color color, bool inOrderOrient = true) {
        vx.push_back(vertexes[0]);
        vx.push_back(vertexes[1]);
        vx.push_back(vertexes[2]);
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

    int  orientate(vector3d normal) {

    };
};

#endif // PRIMITIVE3D_TRIANGLE_H
