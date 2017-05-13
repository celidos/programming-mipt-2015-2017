#include "rt_raytracer.h"


void RayTracer::readffViewport(std::ifstream f) {
    string str;
    while (fin) {
        DDOUBLE x, y, z;
        getline(f, str);
        stringstream ss(str);
        string key;
        ss >> key;
        if (key == "endviewport")
            return;
        else {
            if (key == "origin") {
                ss >> x >> y >> z;
                camera.spt.set(x, y, z);
            }
            else if (key == "topleft") {
                ss >> x >> y >> z;
                camera.TL.set(x, y, z);
            }
            else if (key == "bottomleft") {
                ss >> x >> y >> z;
                camera.BL.set(x, y, z);
            }
            else if (key == "topright") {
                ss >> x >> y >> z;
                camera.TR.set(x, y, z);
            }
            else if (key == "xres")
                ss >> camera.xResolution;
            else if (key == "yres")
                ss >> camera.yResolution;
        }
    }
}

void RayTracer::readffMaterials(std::ifstream f) {
    string str;
    DDOUBLE x, y, z;
    unsigned int r, g, b, alph;

    while (fin) {
        getline(f, str);
        stringstream ss(str);
        string key;
        ss >> key;
        Material material;

        if (key == "endentry")
            scene.materials.push_back(material);
        else if (key == "name")
            ss >> material.name;
        else if (key == "color") {
            ss >> r >> g >> b;
            material.color.from3chars(r, g, b);
        }
        else if (key == "alpha") {
            ss >> alph;
            material.color.setAchar(alph);
        }
        else if (key == "reflect") {
            ss >> material.reflect;
        }
        else if (key == "refract") {
            ss >> material.refract;
        }
        else if (key == "endmaterials")
            return;
    }
}

void RayTracer::readffLights(std::ifstream f) {
    string str;
    DDOUBLE x, y, z;
    unsigned int r, g, b, alph;
    int inReference = 0;
    while (fin) {
        getline(f, str);
        stringstream ss(str);
        string key;
        ss >> key;
        Lighter light;

        if (key == "endlights")
            return;
        else if (key == "reference")
            inReference = 1;
        else if (key == "endreference") {
            inReference = 0;
            scene.lightsystem.lighters.push_back(light);
        }
        else if (key == "point")
            inReference = 2;
        else if (key == "endpoint")
            inReference = 0;
        else if (key == "power" && inReference == 1)
            ss >> scene.lightsystem.scaleI;
        else if (key == "distance" && inReference == 1)
            ss >> scene.lightsystem.scaleDist;
        else if (key == "coords" && inReference == 2) {
            ss >> x >> y >> z;
            light.spot.set(x, y, z);
        }
        else if (key == "power" && inReference == 2)
            ss >> light.I;
    }
}

void RayTracer::readffGeometry(std::ifstream f) {
    string str;
    DDOUBLE x, y, z;
    unsigned int r, g, b, alph;
    int inPrimitive = 0;
    int vxcounter = 0;
    Sphere3d sphere;
    Triangle3d triangle;
    while (fin) {
        getline(f, str);
        stringstream ss(str);
        string key;
        ss >> key;

        if (key == "endgeometry")
            return;
        else if (key == "sphere")
            inPrimitive = 1;
        else if (key == "endsphere") {
            inPrimitive = 0;
            scene.objects.push_back(new Sphere3d(sphere));
        }
        else if (key == "coords" && inPrimitive == 1) {
            ss >> x >> y >> z;
            sphere.c.set(x, y, z);
        }
        else if (key == "radius" && inPrimitive == 1)
            ss >> sphere.r;
        else if (key == "material" && inPrimitive == 1)
            ss >> sphere.materialId;
        else if (key == "triangle") {
            inPrimitive = 2;
            vxcounter = 0;
        }
        else if (key == "endtriangle") {
            inPrimitive = 0;
            if (vxcounter == 3)
                scene.objects.push_back(new Triangle3d(triangle));
        }
        else if (key == "vertex" && inPrimitive == 2) {
            ss >> x >> y >> z;
            triangle.vx[vxcounter % 3].set(x, y, z);
        }
        else if (key == "material" && inPrimitive == 2)
            ss >> triangle.materialId;
    }
}

void RayTracer::readFromRtFile(std::string filename) {
    ifstream fin(filename);

    string str;
    while (fin) {
        getline(fin, str);
        stringstream ss(str);
        string key;

        ss >> key;
        if (key == "viewport")
            readffViewport(fin);
        else if (key == "materials")
            readffMaterials(fin);
        else if (key == "lights")
            readffLights(fin);
        else if (key == "geometry")
            readffGeometry(fin);
    }

    fin.close();
}

Color** RayTracer::render()
{
    uint xResolution = camera.xRes();
    uint yResolution = camera.yRes();

    Color **bitmap = new Color * [xResolution];

    for (uint xPixel = 0; xPixel < xResolution; ++xPixel) {
        bitmap[xPixel] = new Color [yResolution];
        for (uint yPixel = 0; yPixel < yResolution; ++yPixel) {
            vector3d viaPoint = camera.getVector3dByPixelCoord(xPixel, yPixel, xResolution, yResolution);
            bitmap[xPixel][yPixel] = scene.trace(camera.spt, viaPoint, 1);
        }
    }
    return bitmap;
}

