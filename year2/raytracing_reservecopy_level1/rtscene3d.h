#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "rtgeometry.h"
#include "rtprimitive3d.h"
#include "rtprimitive3d_sphere.h"
#include "rtprimitive3d_triangle.h"

#include <iostream>

using std::cout;
using std::endl;

typedef unsigned int uint;



class RtScene3d
{
public:
    std::vector<Primitive3d *> objects;

    RtScene3d() {

    }

    struct IntersectionResult {
        bool succ;
        vector3d p;
        Color color;
        DDOUBLE dist;
    };

    vector3d outcoming;

    static bool myfunction (IntersectionResult a, IntersectionResult b)
    {
        return a.dist < b.dist;
    }

    IntersectionResult shotRay(vector3d &a, vector3d &b, Primitive3d * obj) {
        IntersectionResult res;
        res.succ = false;


        if (obj->type == PT_SPHERE) {
            Sphere3d * s = (Sphere3d*)(obj);
            cout << "center=(" << s->c.x() << "; " << s->c.y()<< " " << s->c.z() << ") ; "
                 << "a=(" << a.x() << "; " << a.y()<< " " << a.z() << ") ; "
                 << "b=(" << b.x() << "; " << b.y()<< " " << b.z() << ") ; " << endl;
            DDOUBLE dst = distPointLine(s->c, a, b);
            cout << dst << "; " << endl;
            if (dst > s->r)
            {
                return res;
            }

            vector3d cc = projPointLine(s->c, a, b);
            vector3d p1 = cc - (b-a).norm() * sqrt(sq(s->r) - sq(dst));

            if ((p1 - b) / (b - a) > 0) // сонаправлены
            {
                res.p = p1;
                res.dist = dist(p1, b);
                res.color = s->color;
                res.succ = true;
            }
            return res;
        }
        else if (obj->type == PT_TRIANGLE) {
            //cout << "KEK" << endl;
            Triangle3d * tr = (Triangle3d *)(obj);

            vector3d dir = (b - a).norm();

            // треугольник параллелен направляющей прямой
            if (*tr || dir) {
                cout << "|| fail" << endl;
                return res;
            }

            vector3d p1 = interPointPlain(a, b, tr->vx[0], tr->vx[1], tr->vx[2]);
            cout << "p1(" << p1.x() << "; " << p1.y() << " " << p1.z() << ") "
                 << "v1(" << tr->vx[0].x() << "; " << tr->vx[0].y() << " " << tr->vx[0].z() << ") "
                 << "v2(" << tr->vx[1].x() << "; " << tr->vx[1].y() << " " << tr->vx[1].z() << ") "
                 << "v3(" <<  tr->vx[2].x() << "; " <<  tr->vx[2].y() << " " <<  tr->vx[2].z() << ") " << endl;

            if ((p1 - b) / (b - a) > 0) // сонаправлены
            {
                if (isPointOnTriangle(p1, tr->vx[0], tr->vx[1], tr->vx[2]) == 1)
                {
                    res.succ = true;
                    res.color = tr->color;
                    res.p = p1;
                    res.dist = dist(p1, b);
                }
            }
            return res;
        }
        else
        {
            cout << obj->type << endl;
        }
    }

    Color trace(vector3d spot, vector3d viapoint) {

        Color colorset = Color::black();

        std::vector<IntersectionResult> results;
        for (uint i = 0; i < objects.size(); ++i) {
            IntersectionResult res = shotRay(spot, viapoint, objects[i]);
            if (res.succ)
            {
                results.push_back(res);
                colorset = res.color;
            }
        }
        sort(results.begin(), results.end(), myfunction);
        if (results.size() > 0)
        {
            cout << " --------------------------- (r = " <<results[0].color.r << "; g = " <<results[0].color.g <<"; b = " <<results[0].color.b<< ")" << endl;
            colorset = results[0].color;
        }

        return mixColors(results);
    }

    Color mixColors(std::vector<IntersectionResult> &res) {
        Color col = Color::black();
        for (int i = res.size() - 1; i >= 0; --i) {
            col = col.mix(res[i].color);
        }
        return col;
    }
};



#endif // SCENE_H
