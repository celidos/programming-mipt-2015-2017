#ifndef RT_PRIMITIVE3D_QUADRANGLE_H
#define RT_PRIMITIVE3D_QUADRANGLE_H

#include <vector>

#include "rt_primitive3d.h"

class Quadrangle3d : public Primitive3d
{
private:
    std::vector<vector3d> vx;

public:
    Quadrangle3d() {
        type = PT_QUADRANGLE;
        vx.push_back(vector3d());
        vx.push_back(vector3d());
        vx.push_back(vector3d());
        vx.push_back(vector3d());
    }

    Quadrangle3d(const Quadrangle3d &obj) : Primitive3d(obj) {
        type = PT_QUADRANGLE;
        vx = obj.vx;
    }

    Quadrangle3d(vector3d A, vector3d B, vector3d C, vector3d D, Material *material, bool inOrderOrient = true) {
        vx.push_back(A);
        vx.push_back(B);
        vx.push_back(C);
        vx.push_back(D);
        orient = inOrderOrient;
        type = PT_QUADRANGLE;
        this->material = material;
    }

    Quadrangle3d(vector3d vertexes[4], Material *material, bool inOrderOrient = true) {
        vx.push_back(vertexes[0]);
        vx.push_back(vertexes[1]);
        vx.push_back(vertexes[2]);
        vx.push_back(vertexes[4]);
        orient = inOrderOrient;
        type = PT_QUADRANGLE;
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

    vector3d& D() {
        return vx[3];
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

#endif // RT_PRIMITIVE3D_QUADRANGLE_H
