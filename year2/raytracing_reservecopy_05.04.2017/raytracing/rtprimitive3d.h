#ifndef PRIMITIVE3D_H
#define PRIMITIVE3D_H

#include <string>

#include "rtgeometry.h"
#include "rtcolor.h"

enum PrimitiveType {PT_BASE, PT_SPHERE, PT_TRIANGLE};

typedef unsigned long long int pindex;

class Primitive3d
{
private:

public:
    PrimitiveType type;
    Color color;
    float mirror;
    std::string materialId;

    Primitive3d() {
        type = PT_BASE;
        index = indexCounter++;
        handleLighting = true;
        orient = 1.0;
        mirror = 0.0;
    }
    void reorient() {
        orient = -orient;
    }
    pindex getindex() {
        return index;
    }
    bool needLighting() {
        return handleLighting;
    }

    void enableLighting() {
        handleLighting = true;
    }

    void disableLighting() {
        handleLighting = false;
    }

    virtual int orientate(vector3d normal) = 0;

protected:
    DDOUBLE orient;
    pindex index;
    bool handleLighting;

    static pindex indexCounter;
};

#endif // PRIMITIVE3D_H
