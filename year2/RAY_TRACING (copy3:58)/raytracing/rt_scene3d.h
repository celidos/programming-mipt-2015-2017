#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "rt_geometry.h"
#include "rt_primitive3d.h"
#include "rt_primitive3d_sphere.h"
#include "rt_primitive3d_triangle.h"
#include "rt_lighting.h"

#include <iostream>
#include <string>

typedef unsigned int uint;

enum RtRayShotCollisionSeacrhMethod {RT_ON_RAY_CONTINUATION, RT_ON_SEGMENT};

class RtScene3d
{
private:
    struct IntersectionResult {
        bool succ;
        vector3d p;
        Color color;
        DDOUBLE dist;
        vector3d normal;
        pindex primitiveind;
        bool needlighting;
        float mirroreffect;
        float refractEffect;

        IntersectionResult();
        void print() {
            std::cout << "InterResult[succ: " << succ << ", p" << p.toStr() << ", "
                      << "color" << color.toStr() << ", dist: " << dist << std::endl;
        }
    };

    vector3d outcoming;
    static bool myfunction (IntersectionResult a, IntersectionResult b);
    Color mixColors(std::vector<IntersectionResult> &res);

    IntersectionResult getClosestSurface(vector3d spot, vector3d viapoint);

    const static Material DEBUG_LAMP_MATERIAL;

public:
    std::vector<Primitive3d *> objects;
    std::vector<Material> materials;
    LightSystem lightsystem;

    // ----

    RtScene3d() = default;

    void setMaterials(std::vector<Material> newmaterials);
    void connectMaterialsWithObjects();

    IntersectionResult shotRay(vector3d a, vector3d b, Primitive3d * obj,
                               RtRayShotCollisionSeacrhMethod method);

    float traceLight(IntersectionResult res, vector3d cameraspot);
    Color traceImaginaryLight(IntersectionResult res, vector3d cameraspot);

    Color trace(vector3d spot, vector3d viapoint, int traceIterations);

    // debug output

    void makeLampsVisible();
    void printMaterials();
    void printGeometry();
    void printImLights();
};

#endif // SCENE_H
