#include "rt_scene3d.h"

RtScene3d::IntersectionResult::IntersectionResult() :
    succ(false), dist(INFINITY_LIMIT), color(Color::black())
{}

bool RtScene3d::myfunction(RtScene3d::IntersectionResult a, RtScene3d::IntersectionResult b)
{
    return a.dist < b.dist;
}

RtScene3d::IntersectionResult RtScene3d::shotRay(vector3d &a, vector3d &b, Primitive3d *obj, RtRayShotCollisionSeacrhMethod method) {

    IntersectionResult res;
    res.succ = false;


    if (obj->type == PT_SPHERE) {
        Sphere3d * s = (Sphere3d*)(obj);
        DDOUBLE dst = distPointLine(s->c, a, b);

        if (dst > s->r)
        {
            return res;
        }

        vector3d cc = projPointLine(s->c, a, b);
        vector3d p1 = cc - (b-a).norm() * sqrt(sq(s->r) - sq(dst));

        DDOUBLE ratio1 = (p1 - b) / (b - a);
        DDOUBLE ratio2 = (p1 - b) / (p1 - a);
        if ((ratio1 > 0.0 && method == RT_ON_RAY_CONTINUATION) ||
                (ratio2 < 0.0 && method == RT_ON_SEGMENT ))
        {
            res.p = p1;
            res.dist = dist(p1, b);
            res.color = s->color;
            res.succ = true;
            res.normal = p1 - s->c;//
            res.primitiveind = obj->getindex();
            res.needlighting = obj->needLighting();
            res.mirroreffect = obj->mirror;
        }
        return res;
    }
    else if (obj->type == PT_TRIANGLE) {

        Triangle3d * tr = (Triangle3d *)(obj);

        vector3d dir = (b - a).norm();

        if (*tr || dir) {
            return res;
        }

        vector3d p1 = interLinePlain(a, b, tr->vx[0], tr->vx[1], tr->vx[2]);

        DDOUBLE ratio1 = (p1 - b) / (b - a);
        DDOUBLE ratio2 = (p1 - b) / (p1 - a);
        if ((ratio1 > 0.0 && method == RT_ON_RAY_CONTINUATION) ||
                (ratio2 < 0.0 && method == RT_ON_SEGMENT))
        {
            if (isPointOnTriangle(p1, tr->vx[0], tr->vx[1], tr->vx[2]) == 1)
            {
                res.succ = true;
                res.color = tr->color;
                res.p = p1;
                res.dist = dist(p1, b);
                res.normal = tr->normal();
                res.primitiveind = obj->getindex();
                res.needlighting = obj->needLighting();
                res.mirroreffect = obj->mirror;
            }
        }
        return res;
    }
    else
    {
        cout << obj->type << endl;
    }
}


void RtScene3d::traceLight(std::vector<RtScene3d::IntersectionResult> &res, vector3d cameraspot) {
    for (int i = res.size() - 1; i >= 0; --i) {
        if (res[i].needlighting)
        {
            vector3d point = res[i].p;
            vector3d normal = res[i].normal;

            for (int j = 0; j < objects.size(); ++j)
            {

                if (isOnTheOneSide(point, normal, lightsystem.spot, cameraspot) == 1)
                {
                    if (res[i].primitiveind != objects[i]->getindex())
                    {
                        IntersectionResult resshot = shotRay(point, lightsystem.spot, objects[j], RT_ON_SEGMENT);
                        if (resshot.succ)
                        {
                            res[i].color = res[i].color.brighter(-0.3);// = Color::black();
                            break;
                            //colorset = res.color;
                        }
                    }

                    float maxI = lightsystem.I/(lightsystem.spot - point).len2();
                    float k = -lightsystem.I/(lightsystem.spot - point).len2() * cosangle(lightsystem.spot - point, res[i].normal);
                    // << "k === " << k << endl;
                    res[i].color.brighter(/*light.I/(light.spot - point).len()*/ 0.3*cosangle(lightsystem.spot - point, res[i].normal));

                }
                else
                {
                    res[i].color = res[i].color.brighter(-0.3); //res[i].color = Color::black();
                    break;
                }
            }
        }
    }
}

Color RtScene3d::mixColors(std::vector<RtScene3d::IntersectionResult> &res) {
    Color col = Color::black();

    for (int i = res.size() - 1; i >= 0; --i)
        col = col.mix(res[i].color);

    return col;
}

RtScene3d::IntersectionResult RtScene3d::getClosestSurface()
{
    // by default - bestResult with INF dist and black color
    IntersectionResult bestResult;

    for (uint i = 0; i < objects.size(); ++i) {
        IntersectionResult res = shotRay(spot, viapoint, objects[i], RT_ON_RAY_CONTINUATION);
        if (res.succ)
            if (res.dist < bestResult.dist)
                bestResult = res;
    }

    return bestResult;
}

Color RtScene3d::trace(vector3d spot, vector3d viapoint, int traceIterations) {

    Color resultColor = Color::black();

    // Most hard operation - Losing time there
    IntersectionResult intersectResult = getClosestSurface();

    if (results.size() > 0) {
        traceLight(results, spot);

        if (traceIterations)
        {
            for (int j = 0; j < results.size(); ++j)
            {
                Color mirrorColor = trace(results[j].p,results[j].p + reflectVector(spot - viapoint, results[j].normal).norm()*0.00001, traceIterations - 1);
                mirrorColor.a = results[j].mirroreffect;
                results[j].color = results[j].color.mix(mirrorColor);
            }
        }
    }

    return mixColors(results);
    }
}
