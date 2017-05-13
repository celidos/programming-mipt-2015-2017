#ifndef RTLIGHTING_H
#define RTLIGHTING_H

#include <vector>
#include "rtgeometry.h"

class LightSystem
{
private:

public:
    std::vector<vector3d> spots;
    std::vector<float> I;
};

#endif // RTLIGHTING_H
