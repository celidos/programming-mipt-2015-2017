#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#include "rtgeometry.h"
#include "rtprimitive3d_sphere.h"
#include "rtcamera3d.h"
#include "rt_scene3d.h"
#include "rtlighting.h"

using std::string;
using std::ifstream;
using std::stringstream;


class RayTracer
{
private:
    void readffViewport(ifstream f);
    void readffMaterials(ifstream f);
    void readffLights(ifstream f);
    void readffGeometry(ifstream f);

public:
    RtCamera3d camera;
    RtScene3d scene;

    RayTracer(RtCamera3d newcamera, RtScene3d newscene) :
        camera(newcamera), scene(newscene)
    { }

    void readFromRtFile(string filename);

    Color ** render();
};

#endif // RAYTRACING_H
