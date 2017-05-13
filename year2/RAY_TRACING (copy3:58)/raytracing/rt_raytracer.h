#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#include "rt_geometry.h"
#include "rt_primitive3d_sphere.h"
#include "rt_camera3d.h"
#include "rt_scene3d.h"
#include "rt_lighting.h"

using std::string;
using std::ifstream;
using std::stringstream;


class RayTracer
{
private:
    bool strIsUninformative(std::string str);

    void readffViewport(std::ifstream &f);
    void readffMaterials(ifstream &f);
    void readffLights(ifstream &f);
    void readffGeometry(ifstream &f);

    void buildImaginaryPart();

public:
    RtCamera3d camera;
    RtScene3d scene;

    RayTracer() = default;
    RayTracer(RtCamera3d newcamera, RtScene3d newscene) :
        camera(newcamera), scene(newscene)
    { }

    void readFromRtFile(string filename);

    Color ** render();

    // debug output

};

#endif // RAYTRACING_H
