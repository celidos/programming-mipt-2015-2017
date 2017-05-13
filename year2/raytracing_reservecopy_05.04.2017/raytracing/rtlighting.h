#ifndef RTLIGHTING_H
#define RTLIGHTING_H

#include <vector>
#include "rtgeometry.h"

class Lighter {

public:
    vector3d spot;
    float I;
    Lighter() :
        spot(vector3d(0.0, 0.0, 0.0)), I(0.0)
    { }

    Lighter(vector3d newspot, float newI) :
        spot(newspot), I(newI)
    { }
};

class LightSystem
{
private:

public:
    std::vector <Lighter> lighters;

    float scaleI;
    float scaleDist;

    LightSystem() {

    }

    LightSystem(std::vector <Lighter> newlighters) :
        scaleI(1.0), scaleDist(1.0)
    {
        lighters = newlighters;
    }
};

#endif // RTLIGHTING_H
