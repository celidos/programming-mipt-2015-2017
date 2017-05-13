#ifndef RTLIGHTING_H
#define RTLIGHTING_H

#include <vector>
#include <iostream>
#include "rt_geometry.h"

class Lighter {

public:
    vector3d spot;
    float I;
    bool debugShowLamp;

    Lighter() :
        spot(vector3d(0.0, 0.0, 0.0)), I(0.0), debugShowLamp(true)
    { }

    Lighter(vector3d newspot, float newI, bool debugShow = true) :
        spot(newspot), I(newI), debugShowLamp(debugShow)
    { }
};

class LightSystem
{
private:

public:
    std::vector <Lighter> lighters;

    float scaleI;
    float scaleDist;
    float greatSun;

    LightSystem() {
        greatSun = 0.1;
    }

    LightSystem(std::vector <Lighter> newlighters) :
        scaleI(1.0), scaleDist(1.0), greatSun(0.1)
    {
        lighters = newlighters;

    }

    // debug output

    void print();
};

#endif // RTLIGHTING_H
