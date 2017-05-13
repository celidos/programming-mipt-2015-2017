#include "rt_raytracer.h"


bool RayTracer::strIsUninformative(std::__cxx11::string str)
{
    for (uint i = 0; i < str.size(); ++i) {
        if (str[i] == '#')
            return true;
        if (str[i] != ' ' || str[i] != '\t' || str[i] != '\n' || str[i] != '\r')
            return false;
    }
    return true;
}

void RayTracer::readffViewport(std::ifstream &f) {
    string str;
    while (f) {
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
                camera.spot.set(x, y, z);
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

void RayTracer::readffMaterials(std::ifstream &f) {
    string str;
    unsigned int r, g, b;
    float alpha;
    Material material;
    while (f) {
        getline(f, str);
        if (strIsUninformative(str)) continue;
        stringstream ss(str);
        string key;
        ss >> key;

        if (key == "endentry")
            scene.materials.push_back(material);
        else if (key == "name") {
            ss >> key;
            material.setId(key);
        }
        else if (key == "color") {
            ss >> r >> g >> b;
            material.color.set3i(r, g, b);
        }
        else if (key == "alpha") {
            ss >> alpha;
            material.color.setAf(alpha);
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

void RayTracer::readffLights(std::ifstream &f) {
    string str;
    DDOUBLE x, y, z;
    int inReference = 0;
    Lighter light;
    while (f) {
        getline(f, str);
        if (strIsUninformative(str)) continue;
        stringstream ss(str);
        string key;
        ss >> key;

        if (key == "endlights")
            return;
        else if (key == "reference")
            inReference = 1;
        else if (key == "endreference")
            inReference = 0;
        else if (key == "point")
            inReference = 2;
        else if (key == "endpoint") {
            inReference = 0;
            scene.lightsystem.lighters.push_back(light);
        }
        else if (key == "power" && inReference == 1)
            ss >> scene.lightsystem.scaleI;
        else if (key == "distance" && inReference == 1)
            ss >> scene.lightsystem.scaleDist;
        else if (key == "greatsun" && inReference == 1)
            ss >> scene.lightsystem.greatSun;
        else if (key == "coords" && inReference == 2) {
            ss >> x >> y >> z;
            light.spot.set(x, y, z);
        }
        else if (key == "power" && inReference == 2)
            ss >> light.I;
    }
}

void RayTracer::readffGeometry(std::ifstream &f) {
    string str;
    DDOUBLE x, y, z;
    int inPrimitive = 0;
    int vxcounter = 0;
    Sphere3d sphere;
    Triangle3d triangle;
    while (f) {
        getline(f, str);
        if (strIsUninformative(str)) continue;
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
        else if (key == "material" && inPrimitive == 1) {
            ss >> key;
            sphere.setMaterialId(key);
        }
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
            triangle[vxcounter % 3].set(x, y, z);
            ++vxcounter;
        }
        else if (key == "material" && inPrimitive == 2) {
            ss >> key;
            triangle.setMaterialId(key);
        }
    }
}

void RayTracer::buildImaginaryPart()
{
    for (uint i = 0; i < scene.lightsystem.lighters.size(); ++i) {
        for (uint j = 0; j < scene.objects.size(); ++j) {
            scene.lightsystem.imlighters.push_back(ImaginaryLighter(scene.lightsystem.lighters[i], scene.objects[j]));
        }
    }
}

void RayTracer::readFromRtFile(std::string filename) {
    ifstream fin(filename);

    string str;
    while (fin) {
        getline(fin, str);
        if (strIsUninformative(str)) continue;
        stringstream ss(str);
        string key;

        ss >> key;
        if (key == "viewport") {
            readffViewport(fin);
            camera.print();
        }
        else if (key == "materials") {
            readffMaterials(fin);
            scene.printMaterials();
        }
        else if (key == "lights") {
            readffLights(fin);
            scene.lightsystem.print();
        }
        else if (key == "geometry") {
            readffGeometry(fin);
        }
    }
    fin.close();

    scene.connectMaterialsWithObjects();

    buildImaginaryPart();
    scene.printImLights();

    //scene.makeLampsVisible();
    scene.printGeometry();
}

struct ToThreadParams
{
    RayTracer *rt;
    int x;
    int processCount;
    ToThreadParams(RayTracer *newrt, int X, int processCount)
    {
        rt = newrt;
        x = X;
        this->processCount = processCount;
    };
};

void* CalculateThread(void * arg)
{
    ToThreadParams *P = (ToThreadParams *) arg;
    RayTracer *rt = P->rt;

    rt->renderline(P->x, P->processCount);
}

void RayTracer::renderline(int xStart, int step)
{
    uint xResolution = camera.xRes();
    uint yResolution = camera.yRes();
    float scale = 1.0 / (antiAliasingXrate * antiAliasingYrate);

    for (uint xPixel = xStart; xPixel < xResolution; xPixel += step) {
        bitmap[xPixel] = new Color [yResolution];
        for (uint yPixel = 0; yPixel < yResolution; ++yPixel) {
            float rsum = 0.0, gsum = 0.0, bsum = 0.0;
            for (uint aaX = 0; aaX < antiAliasingXrate; ++aaX)  {
                for (uint aaY = 0; aaY < antiAliasingYrate; ++aaY) {
                    vector3d viaPoint = camera.getVector3dByPixelCoord(xPixel*antiAliasingXrate + aaX,
                                                                       yPixel*antiAliasingYrate + aaY,
                                                                       xResolution*antiAliasingXrate,
                                                                       yResolution*antiAliasingYrate);
                    Color color =  scene.trace(camera.spot, viaPoint, 4);
                    rsum += color.r;
                    gsum += color.g;
                    bsum += color.b;
                }
            }

            bitmap[xPixel][yPixel] = Color(rsum * scale, gsum * scale, bsum * scale);///scene.trace(camera.spot, viaPoint, 5);
        }
    }
}

Color** RayTracer::render()
{
    uint xResolution = camera.xRes();
    uint yResolution = camera.yRes();

    bitmap = new Color * [xResolution];
    for (uint xPixel = 0; xPixel < xResolution; ++xPixel)
        bitmap[xPixel] = new Color [yResolution];

    ToThreadParams p1 = ToThreadParams( this, 0, 4 ),
                   p2 = ToThreadParams( this, 1, 4 ),
                   p3 = ToThreadParams( this, 2, 4 ),
                   p4 = ToThreadParams( this, 3, 4 );
    int result;
    pthread_t thread1, thread2, thread3, thread4;

    result = pthread_create(&thread1, NULL, CalculateThread, &p1);
    if (result != 0)
        throw EXIT_FAILURE;

    result = pthread_create(&thread2, NULL, CalculateThread, &p2);
    if (result != 0)
        throw EXIT_FAILURE;

    result = pthread_create(&thread3, NULL, CalculateThread, &p3);
    if (result != 0)
        throw EXIT_FAILURE;

    result = pthread_create(&thread4, NULL, CalculateThread, &p4);
    if (result != 0)
        throw EXIT_FAILURE;

    // joins

    result = pthread_join(thread1, NULL);
    if (result != 0) {
        throw EXIT_FAILURE;
    }
    result = pthread_join(thread2, NULL);
    if (result != 0) {
        throw EXIT_FAILURE;
    }
    result = pthread_join(thread3, NULL);
    if (result != 0) {
        throw EXIT_FAILURE;
    }
    result = pthread_join(thread4, NULL);
    if (result != 0) {
        throw EXIT_FAILURE;
    }

    return bitmap;
}

