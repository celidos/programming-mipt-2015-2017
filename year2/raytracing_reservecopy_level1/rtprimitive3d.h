#ifndef PRIMITIVE3D_H
#define PRIMITIVE3D_H

#include "rtgeometry.h"
#include "rtcolor.h"

enum PrimitiveType {PT_BASE, PT_SPHERE, PT_TRIANGLE};

class Primitive3d
{
public:
    PrimitiveType type;
    Color color;

    Primitive3d() {
        type = PT_BASE;
    }

    void reorient() {
        orient = !orient;
    }
    virtual int  orientate(vector3d normal) = 0;

protected:
    bool orient;
};

#endif // PRIMITIVE3D_H
