#include "rt_scene3d.h"

const Material RtScene3d::DEBUG_LAMP_MATERIAL = Material(Color::yellow(), "_DEBUGLAMPMATERIAL", 1.0, 1.0);

RtScene3d::IntersectionResult::IntersectionResult() :
    succ(false), color(Color::black()), dist(INFINITY_LIMIT)
{}

bool RtScene3d::myfunction(RtScene3d::IntersectionResult a, RtScene3d::IntersectionResult b)
{
    return a.dist < b.dist;
}

RtScene3d::IntersectionResult RtScene3d::shotRay(vector3d a, vector3d b, Primitive3d *obj, RtRayShotCollisionSeacrhMethod method) {

    IntersectionResult res;
    res.succ = false;

    if (obj->getType() == PT_SPHERE) {
        Sphere3d * s = (Sphere3d*)(obj);
        DDOUBLE dst = distPointLine(s->c, a, b);

        if (dst > s->r)
        {
            return res;
        }

        vector3d dir = (b - a).norm();
        vector3d cc = projPointLine(s->c, a, b);

        vector3d p1;
        if (dist2(a, s->c) > s->r) {
            p1 = cc - dir * sqrt(sq(s->r) - sq(dst));
        } else {
            p1 = cc + dir * sqrt(sq(s->r) - sq(dst));
        }


        DDOUBLE ratio1 = (p1 - a) / (b - a);
        DDOUBLE ratio2 = (p1 - b) / (p1 - a);
        if ((ratio1 > ZERO_LIMIT && method == RT_ON_RAY_CONTINUATION) ||
                (ratio2 < -ZERO_LIMIT && method == RT_ON_SEGMENT ))
        {
            res.p = p1;
            res.dist = dist(p1, a);
            res.color = s->getColor();
            res.succ = true;
            res.normal = (p1 - s->c).norm();
            res.primitiveind = obj->getIndex();
            res.needlighting = obj->needLighting();
            res.mirroreffect = obj->getReflect();
            res.refractEffect = obj->getRefract();
        }
        return res;
    }
    else if (obj->getType() == PT_TRIANGLE) {

        Triangle3d * tr = (Triangle3d *)(obj);

        vector3d dir = (b - a).norm();

        if (*tr || dir) {
            return res;
        }

        vector3d p1 = interLinePlain(a, b, tr->A(), tr->B(), tr->C());

        DDOUBLE ratio1 = (p1 - a) / (b - a);
        DDOUBLE ratio2 = (p1 - b) / (p1 - a);
        if ((ratio1 > 0.0 && method == RT_ON_RAY_CONTINUATION) ||
                (ratio2 < 0.0 && method == RT_ON_SEGMENT))
        {
            if (isPointOnTriangle(p1, tr->A(), tr->B(), tr->C()) == 1)
            {
                res.succ = true;
                res.color = tr->getColor();
                res.p = p1;
                res.dist = dist(p1, a);
                res.normal = tr->normal();
                res.primitiveind = obj->getIndex();
                res.needlighting = obj->needLighting();
                res.mirroreffect = obj->getReflect();
                res.refractEffect = obj->getRefract();
            }
        }
        return res;
    }
    else if (obj->getType() == PT_BASE) {

    }
    return res;
}


float RtScene3d::traceLight(IntersectionResult res, vector3d cameraspot)
{
    vector3d &point = res.p;
    vector3d &normal = res.normal;

    float resultI = 0.0;

    for (uint iterL = 0; iterL < lightsystem.lighters.size(); ++iterL) {
        vector3d &lightspot = lightsystem.lighters[iterL].spot;

        bool flagIsOk = true;
        if (isOnTheOneSide(point, normal, lightsystem.lighters[iterL].spot, cameraspot) == 1) {

            for (uint j = 0; j < objects.size() && flagIsOk; ++j) {
                if (objects[j]->needLightNoClip())
                    continue;

                //if (objects[j]->getIndex() != res.primitiveind) {
                {
                    IntersectionResult shotResult = shotRay(point + (lightspot - point).norm() * ZERO_LIMIT_2ORDER, lightspot,
                                                            objects[j], RT_ON_SEGMENT);
                    if (shotResult.succ) {
                        flagIsOk = false;
                        break;
                    }
                }
            }
        }
        else
            flagIsOk = false;

        if (flagIsOk) {
            resultI += sq(lightsystem.lighters[iterL].I * lightsystem.scaleI / lightsystem.scaleDist) / dist2(res.p, lightspot)
                    * fabs(cosangle2v(lightspot - point, normal));
        }
    }

    return resultI;
}

Color RtScene3d::traceImaginaryLight(IntersectionResult res, vector3d cameraspot) {
    vector3d &point = res.p;
    vector3d dirFromCamera = (point - cameraspot).tolen(ZERO_LIMIT_2ORDER);
    vector3d &normal = res.normal;

    Color result(0, 0, 0, 0);

    for (uint iterL = 0; iterL < lightsystem.imlighters.size(); ++iterL) {

        ImaginaryLighter &ilight = lightsystem.imlighters[iterL];
        if (res.primitiveind != ilight.obj->getIndex()) {
            vector3d dirToLight = (ilight.spot - point).norm();
            IntersectionResult objInter = //shotRay(point - dirFromCamera + dirToLight*ZERO_LIMIT_2ORDER,
                                           //       point - dirFromCamera + dirToLight, ilight.obj, RT_ON_RAY_CONTINUATION);
                    getClosestSurface(point - dirFromCamera + dirToLight*ZERO_LIMIT_2ORDER, point - dirFromCamera + dirToLight);
            if (objInter.succ) {

                if (objInter.primitiveind == ilight.obj->getIndex())
                {
                    vector3d objPoint = objInter.p;
                    //std::cout << "inter succ!" << std::endl;
                    vector3d startpoint = objPoint - dirToLight * ZERO_LIMIT_2ORDER;

                    bool isOk = true;
                    for (uint t = 0; t < objects.size(); ++t)
                    {

                        IntersectionResult lightChk = shotRay(startpoint, ilight.lighter->spot, objects[t], RT_ON_SEGMENT);
                        if (lightChk.succ) {
                            //std::cout << "intersection startpoint" << startpoint.toStr() << ", to" << ilight.lighter->spot.toStr() << ", "
                            //          << "between " <<  lightChk.primitiveind << " and " << ilight.obj->getIndex() << std::endl;
                            //if (lightChk.primitiveind != ilight.obj->getIndex())
                            {
                                isOk = false;
                                break;
                            }
                        }
                    }

                    if (isOk) {
                        //std::cout << "OK!" << std::endl;
                        DDOUBLE ddist = objInter.dist;

                        //sq(lightsystem.lighters[iterL].I * lightsystem.scaleI / lightsystem.scaleDist) / dist2(res.p, lightspot) * fabs(cosangle2v(lightspot - point, normal));

                        result.r += ilight.obj->getColor().r * sq(ilight.I * lightsystem.scaleI / (lightsystem.scaleDist))
                                / dist2(point, ilight.spot) * fabs(cosangle2v(ilight.spot - point, normal));
                        result.g += ilight.obj->getColor().g * sq(ilight.I * lightsystem.scaleI / (lightsystem.scaleDist))
                                / dist2(point, ilight.spot) * fabs(cosangle2v(ilight.spot - point, normal));
                        result.b += ilight.obj->getColor().b * sq(ilight.I * lightsystem.scaleI / (lightsystem.scaleDist))
                                / dist2(point, ilight.spot) * fabs(cosangle2v(ilight.spot - point, normal));
                    }
                }
            }
        }
    }
    return result;
}

Color RtScene3d::mixColors(std::vector<RtScene3d::IntersectionResult> &res) {
    Color col = Color::black();

    for (int i = res.size() - 1; i >= 0; --i)
        col = col.mix(res[i].color, res[i].color.a);

    return col;
}

RtScene3d::IntersectionResult RtScene3d::getClosestSurface(vector3d spot, vector3d viapoint)
{
    // by default - bestResult with INF dist and black color
    IntersectionResult bestResult;

    for (uint i = 0; i < objects.size(); ++i) {
        IntersectionResult res = shotRay(spot, viapoint, objects[i], RT_ON_RAY_CONTINUATION);
        // std::cout << "*";
        // res.print();
        // std::cout << (objects[i]->getType()) << std::endl;
        if (res.succ)
            if (res.dist < bestResult.dist)
                bestResult = res;
    }

    return bestResult;
}

void RtScene3d::connectMaterialsWithObjects()
{
    for (uint iterObj = 0; iterObj < objects.size(); ++iterObj) {
        std::string searchingFor = objects[iterObj]->getMaterialId();
        for (uint iterMat = 0; iterMat < materials.size(); ++iterMat) {
            if (searchingFor == materials[iterMat].getId()) {
                std::cout << "New connection between obj and mat!\n";
                objects[iterObj]->setMaterial(&materials[iterMat]);
            }
        }
    }
}

Color RtScene3d::trace(vector3d spot, vector3d viapoint, int traceIterations) {
//    std::cout << "tracing spot" << spot.toStr() << ", viapoint" << viapoint.toStr() << "\n";

    Color resultColor = Color::black();

    // Most hard operation - Losing time there
    IntersectionResult intersectResult = getClosestSurface(spot, viapoint);
    if (traceIterations <= 0)
        return intersectResult.color;
    //intersectResult.print();

    if (intersectResult.succ) {
        float inPointI;
        if (intersectResult.needlighting)
            inPointI = traceLight(intersectResult, spot);
        else
            return intersectResult.color;



        Color baseColor = intersectResult.color;
        resultColor = baseColor.scale(lightsystem.greatSun).add(baseColor.scale(inPointI));

        // -------------------------------------

        Color diffuseColor = traceImaginaryLight(intersectResult, spot);
        resultColor = resultColor.add(diffuseColor);

        // -------------------------------------
        if (intersectResult.color.a < 1.0 - ZERO_LIMIT)
        {
            if (traceIterations > 0) {
                vector3d refrvect = (viapoint - spot);//.norm();
                bool isOk = true;
                refrvect = refractVector(refrvect, intersectResult.normal.norm(), intersectResult.refractEffect, isOk);
                std::cout << refrvect.norm().toStr() <<std::endl;
                if (isOk)
                {
                    //std::cout << "ok! " << std::endl;
                    Color refractColor = trace(intersectResult.p + refrvect * ZERO_LIMIT_2ORDER,
                                               intersectResult.p + refrvect,
                                               traceIterations - 1);
                    //Color refractColor = Color::black();
                    resultColor = resultColor.mix(refractColor, 1.0 - intersectResult.color.a);//intersectResult.color.mix(refractColor);
                }
            }
        }
        // ------------------------------------

        if (traceIterations > 0) {
            vector3d reflvect =  reflectVector(spot - viapoint, intersectResult.normal).norm();
            Color mirrorColor = trace(intersectResult.p + reflvect * ZERO_LIMIT_2ORDER,
                                      intersectResult.p + reflvect,
                                      traceIterations - 1);
            //mirrorColor.a = intersectResult.mirroreffect;
            resultColor = resultColor.mix(mirrorColor, intersectResult.mirroreffect);
        }
    }

    return resultColor;
}

void RtScene3d::makeLampsVisible()
{
    for (uint i = 0; i < lightsystem.lighters.size(); ++i) {
        if (lightsystem.lighters[i].debugShowLamp) {
            objects.push_back(new Sphere3d(lightsystem.lighters[i].spot, 0.2, &DEBUG_LAMP_MATERIAL));
            objects.back()->disableLighting();
            objects.back()->enableLightNoClip();
        }
    }
}

void RtScene3d::printMaterials()
{
    std::cout << "materials[\n";
    for (uint i = 0; i < materials.size(); ++i) {
        std::cout << "  [" << i << "]: color(" << materials[i].color.r << "; " << materials[i].color.g << "; "
                  << materials[i].color.b << ", a = " << materials[i].color.a << ") " << "name: '" << materials[i].getId()
                  << "', refl: " << materials[i].reflect << ", refr: " << materials[i].refract << std::endl;
    }
    std::cout << "]\n";
}

void RtScene3d::printGeometry()
{
    std::cout << "objects[\n";
    std::cout << "  total " << objects.size() << " objects\n";
    for (uint i = 0; i < objects.size(); ++i) {
        std::cout << "  [" << i << "]: id: " << objects[i]->getIndex() << ", type: " << objects[i]->getType() << "; material = "
                  << (objects[i]->isEmptyMaterial() ? "null" : "(...)") << "\n";
    }
    std::cout << "]" << std::endl;
}

void RtScene3d::printImLights()
{
    std::cout << "imLights[\n";
    std::cout << "  total " << lightsystem.imlighters.size() << " objects\n";
    for (uint i = 0; i < lightsystem.imlighters.size(); ++i) {
        std::cout << "  [" << i << "]: spot" << lightsystem.imlighters[i].spot.toStr() << "; I: " << lightsystem.imlighters[i].I << "\n";
    }
    std::cout << "]" << std::endl;
}
