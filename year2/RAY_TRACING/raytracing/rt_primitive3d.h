#ifndef PRIMITIVE3D_H
#define PRIMITIVE3D_H

#include "debug_functional.h"

#include "rt_geometry.h"
#include "rt_color.h"
#include "rt_material.h"

enum PrimitiveType {PT_BASE = 100, PT_SPHERE = 102, PT_TRIANGLE = 104, PT_QUADRANGLE = 106};

typedef unsigned long long int pindex;

class Primitive3d
{
protected:
    PrimitiveType type;
    std::string materialId;
    const Material *material;

public:
    Primitive3d() :
        type(PT_BASE), material(nullptr), orient(1.0), index(indexCounter++), handleLighting(true), lightNoClip(false)
    { }

    Primitive3d(const Primitive3d &obj) {
        lightNoClip = obj.lightNoClip;
        type = obj.type;
        material = obj.material;
        materialId = obj.materialId;
        orient = obj.orient;
        index = indexCounter++;
        handleLighting = obj.handleLighting;
    }

    void reorient() {
        orient = -orient;
    }

    pindex getIndex() {
        return index;
    }

    bool needLighting() {
        return handleLighting;
    }

    void enableLighting() {
        handleLighting = true;
    }

    void disableLighting() {
        handleLighting = false;
    }

    bool needLightNoClip() {
        return lightNoClip;
    }

    void enableLightNoClip() {
        lightNoClip = true;
    }

    void disableLightNoClip() {
        lightNoClip = false;
    }

    virtual PrimitiveType getType() {
        return type;
    }

    Color getColor() {
        if (material)
            return material->color;
        return Color::black();
    }

    float getReflect() {
        if (material)
            return material->reflect;
        return Material::MATERIAL_REFLECT_DEFAULT_VALUE;
    }

    float getRefract() {
        if (material)
            return material->refract;
        return Material::MATERIAL_REFRACT_DEFAULT_VALUE;
    }

    void setMaterialId(std::string newname) {
        materialId = newname;
    }

    std::string getMaterialId() {
        return materialId;
    }

    void setMaterial(Material *material = nullptr) {
        this->material = material;
        materialId.clear();
    }

    bool isEmptyMaterial() {
        return (material == nullptr);
    }

    virtual int orientate(vector3d normal) = 0;
    virtual vector3d inversePoint(vector3d spot) = 0;

protected:
    DDOUBLE orient;
    pindex index;
    bool handleLighting;
    bool lightNoClip;

    static pindex indexCounter;
};

#endif // PRIMITIVE3D_H
