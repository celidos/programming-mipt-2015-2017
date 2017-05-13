#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include <string>
#include "rt_color.h"

class Material
{
private:
    std::string id;

public:
    Color color;
    float reflect;
    float refract;

    // ----

    Material() = default;
    Material(Color newcolor, std::string newname, float newreflect = MATERIAL_REFLECT_DEFAULT_VALUE,
             float newrefract = MATERIAL_REFRACT_DEFAULT_VALUE) :
        color(newcolor), id(newname), reflect(newreflect), refract(newrefract)
    { }

    std::string getId() {
        return id;
    }

    void setId(std::string newid) {
        id = newid;
    }

    static const float MATERIAL_REFLECT_DEFAULT_VALUE;
    static const float MATERIAL_REFRACT_DEFAULT_VALUE;
};

#endif // RT_MATERIAL_H
