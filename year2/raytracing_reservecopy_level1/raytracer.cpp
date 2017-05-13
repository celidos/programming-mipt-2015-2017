#include "raytracer.h"

Color** RayTracer::render()
{
    uint xResolution = camera.xRes();
    uint yResolution = camera.yRes();

    Color **bitmap = new Color * [xResolution];

    for (uint xPixel = 0; xPixel < xResolution; ++xPixel) {
        bitmap[xPixel] = new Color [yResolution];
        for (uint yPixel = 0; yPixel < yResolution; ++yPixel) {
            vector3d viaPoint = camera.getVector3dByPixelCoord(xPixel, yPixel, xResolution, yResolution);

            bitmap[xPixel][yPixel] = scene.trace(camera.spt, viaPoint);
        }
    }
    return bitmap;
}
