#include "paintdevice.h"

#include "raytracer.h"

PaintDevice::PaintDevice(QWidget *parent): QWidget(parent)
{
    //setStyleSheet("background: white");
    showrect = false;
}

void PaintDevice::draw()
{
    pict = new QPicture;

    painter =  new QPainter(pict);
    // --------------------


    RtCamera3d camera(vector3d(0, 0, 0), vector3d(-1, 1, 1), vector3d(1, 1, 1), vector3d(-1, 1, -1), 320,240 );
    RtScene3d scene;
    scene.objects.push_back(new Triangle3d(vector3d(-0.5, 3.4, 0.5),
                                           vector3d(-0.5, 1.5, 0.75),
                                           vector3d(-1.55, 1.5, 0.5),
                                           Color(0.0, 1.0, 0.0, 1.0)));
    scene.objects.push_back(new Triangle3d(vector3d(-2, 3.2, 1),
                                           vector3d(1.7, 3.2, 1),
                                           vector3d(-2, 3.2, -3),
                                           Color(1.0, 0.0, 0.0, 0.6)));
    scene.objects.push_back(new Sphere3d(0, 4, 0, 1, Color::blue(), true));
    RayTracer rt(camera, scene);
    Color ** bitmap = rt.render();

    // --------------------

    for (int i = 0; i < 320; ++i)
    {
        for (int j = 0; j < 240; ++j)
        {
            painter->setPen(QColor::fromRgbF(bitmap[i][j].r, bitmap[i][j].g, bitmap[i][j].b));
            painter->drawPoint(i, j);
        }
    }

    // ------------------------------

    painter->end();
    showrect = true;
    repaint();
    delete [] bitmap;
}

void PaintDevice::paintEvent(QPaintEvent *ev)
{
    if (showrect)
    {
        QPainter p;
        p.begin(this);
        p.drawPicture(0, 0, *pict);
        p.end();
        //pict->save("drawing.bmp");
    }
    QWidget::paintEvent(ev);
}
