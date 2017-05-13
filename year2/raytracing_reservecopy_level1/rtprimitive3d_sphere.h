#ifndef PRIMITIVE3D_SPHERE_H
#define PRIMITIVE3D_SPHERE_H

#include "rtprimitive3d.h"

class Sphere3d : public Primitive3d
{
public:
    vector3d c; // center
    DDOUBLE r; // radius

    Sphere3d(vector3d center, DDOUBLE radius, Color color, bool orientOuter=true) {
        type = PT_SPHERE;
        c = center;
        r = radius;
        orient = orientOuter;
        this->color = color;
    }

    Sphere3d(DDOUBLE centerX, DDOUBLE centerY, DDOUBLE centerZ, DDOUBLE radius,
             Color color, bool orientOuter=true) {
        type = PT_SPHERE;
        c = vector3d(centerX, centerY, centerZ);
        r = radius;
        orient = orientOuter;
        this->color = color;
    }

    int orientate(vector3d normal) {

    }
};

#endif // PRIMITIVE3D_SPHERE_H
