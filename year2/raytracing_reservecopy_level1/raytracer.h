#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <vector>
#include <algorithm>
#include "rtgeometry.h"
#include "rtprimitive3d_sphere.h"
#include "rtcamera3d.h"
#include "rtscene3d.h"

class RayTracer
{
public:
    RtCamera3d camera;
    RtScene3d scene;

    Color **render();

    RayTracer(RtCamera3d newcamera, RtScene3d newscene) {
        camera = newcamera;
        scene = newscene;
    }
};

#endif // RAYTRACING_H
