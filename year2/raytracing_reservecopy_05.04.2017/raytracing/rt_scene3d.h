#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "rtgeometry.h"
#include "rtprimitive3d.h"
#include "rtprimitive3d_sphere.h"
#include "rtprimitive3d_triangle.h"
#include "rtlighting.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef unsigned int uint;

enum RtRayShotCollisionSeacrhMethod {RT_ON_RAY_CONTINUATION, RT_ON_SEGMENT};

class Material {
    Color color;
    std::string name;
    DDOUBLE reflect;
    DDOUBLE refract;
    Material(Color newcolor, std::string newname) {
        color = newcolor;
        name = newname;
    }
};

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

        IntersectionResult();
    };

    vector3d outcoming;
    static bool myfunction (IntersectionResult a, IntersectionResult b);
    Color mixColors(std::vector<IntersectionResult> &res);

    IntersectionResult getClosestSurface();

public:
    std::vector<Primitive3d *> objects;
    std::vector<Material> materials;
    LightSystem lightsystem;

    // ----

    RtScene3d() = default;

    void setMaterials(std::vector<Material> newmaterials);

    IntersectionResult shotRay(vector3d &a, vector3d &b, Primitive3d * obj,
                               RtRayShotCollisionSeacrhMethod method);

    void traceLight(std::vector<IntersectionResult> &res, vector3d cameraspot);

    Color trace(vector3d spot, vector3d viapoint, int traceIterations);
};

#endif // SCENE_H
