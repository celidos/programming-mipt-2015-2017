#ifndef PRIMITIVE3D_TRIANGLE_H
#define PRIMITIVE3D_TRIANGLE_H

#include <vector>

#include "rt_primitive3d.h"

class Triangle3d : public Primitive3d
{
private:
    std::vector<vector3d> vx;

public:
    Triangle3d() {
        type = PT_TRIANGLE;
        vx.push_back(vector3d());
        vx.push_back(vector3d());
        vx.push_back(vector3d());
    }

    Triangle3d(const Triangle3d &obj) : Primitive3d(obj) {
        type = PT_TRIANGLE;
        vx = obj.vx;
    }

    Triangle3d(vector3d A, vector3d B, vector3d C, Material *material, bool inOrderOrient = true) {
        vx.push_back(A);
        vx.push_back(B);
        vx.push_back(C);
        orient = inOrderOrient;
        type = PT_TRIANGLE;
        this->material = material;
    }

    Triangle3d(vector3d vertexes[3], Material *material, bool inOrderOrient = true) {
        vx.push_back(vertexes[0]);
        vx.push_back(vertexes[1]);
        vx.push_back(vertexes[2]);
        orient = inOrderOrient;
        type = PT_TRIANGLE;
        this->material = material;
    }

    vector3d& A() {
        return vx[0];
    }

    vector3d& B() {
        return vx[1];
    }

    vector3d& C() {
        return vx[2];
    }

    vector3d& operator[](int i) {
        return vx[i];
    }

    vector3d normal() {
        return ((vx[2] - vx[0]) ^ (vx[1] - vx[0])).norm();
    }

    bool operator||(vector3d direction) {
        return fabs(normal() * direction) < ZERO_LIMIT;
    }

    virtual int  orientate(vector3d normal) {
        return 0;
    };

    virtual vector3d inversePoint(vector3d spot) {
        vector3d proj = projPointPlain(spot, vx[0], vx[1], vx[2]);
        return spot + (proj - spot) * 2.0;
    }
};

#endif // PRIMITIVE3D_TRIANGLE_H
