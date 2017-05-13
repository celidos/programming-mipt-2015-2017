#include "rt_lighting.h"


void LightSystem::print()
{
    std::cout << "lights[\n";
    std::cout << "  power: " << scaleI << "\n  dist: "<< scaleDist << std::endl;
    for (uint i = 0; i < lighters.size(); ++i) {
        std::cout << "  [" << i << "]: spot: (" << lighters[i].spot.x()
                  << ", " << lighters[i].spot.y() << ", " << lighters[i].spot.z() << "), I = "
                  << lighters[i].I << std::endl;
    }
    std::cout << "]" << std::endl;
}
