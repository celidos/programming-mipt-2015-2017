#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include <string>

#include <QImage> // only for loading texture!
#include <QString>
#include <QColor>

#include "rt_color.h"

class Material
{
private:
    std::string id;
    bool hasTexture;
    QImage texture;

public:
    Color color;
    float reflect;
    float refract;

    // ----

    Material() :
        hasTexture(false), texture(nullptr), color(Color::black()), reflect(MATERIAL_REFLECT_DEFAULT_VALUE),
        refract(MATERIAL_REFRACT_DEFAULT_VALUE)
    { }

    Material(Color newcolor, std::string newname, float newreflect = MATERIAL_REFLECT_DEFAULT_VALUE,
             float newrefract = MATERIAL_REFRACT_DEFAULT_VALUE) :
        color(newcolor), id(newname), reflect(newreflect), refract(newrefract)
    { }

    ~Material() {

    }

    void setTexture(std::string filename) {
        texture.load(QString::fromStdString(filename));
        hasTexture = true;
    }

    void deleteTexture() {
        hasTexture = false;
    }

    bool isTextured() {
        return hasTexture;
    }

    Color getPixelByLinear(float scaledX, float scaledY) {
        if (hasTexture) {
            QColor col = texture.pixelColor(round(scaledX * texture.width()), round(scaledY * texture.height()));
            return Color(col.red(), col.green(), col.blue());
        }
        return color;
    }

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
