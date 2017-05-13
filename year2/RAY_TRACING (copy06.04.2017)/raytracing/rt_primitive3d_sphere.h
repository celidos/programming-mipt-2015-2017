#ifndef PRIMITIVE3D_SPHERE_H
#define PRIMITIVE3D_SPHERE_H

#include "rt_primitive3d.h"

class Sphere3d : public Primitive3d
{
public:
    vector3d c; // center
    DDOUBLE r; // radius

    Sphere3d() {
        type = PT_SPHERE;
    }
    Sphere3d(const Sphere3d &obj) : Primitive3d(obj) {
        c = obj.c;
        r = obj.r;
        type = PT_SPHERE;
    }

    Sphere3d(vector3d center, DDOUBLE radius, const Material *material, bool orientOuter=true) :
        c(center), r(radius)
    {

        type = PT_SPHERE;
        this->material = material;
        orient = orientOuter;
    }

    Sphere3d(DDOUBLE centerX, DDOUBLE centerY, DDOUBLE centerZ, DDOUBLE radius, const Material *material,
             bool orientOuter=true) :
        c(vector3d(centerX, centerY, centerZ)), r(radius)
    {
        type = PT_SPHERE;
        this->material = material;
        orient = orientOuter;
    }

    int orientate(vector3d normal) {
        return 0;
    }
};

#endif // PRIMITIVE3D_SPHERE_H
