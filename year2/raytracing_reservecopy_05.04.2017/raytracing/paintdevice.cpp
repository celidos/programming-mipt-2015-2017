#include "paintdevice.h"

#include "rt_raytracer.h"

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

    RayTracer rt;
    rt.readFromRtFile("scene01.rt");
    Color ** bitmap = rt.render();

    // --------------------

    for (int i = 0; i < rt.camera.xRes(); ++i)
    {
        for (int j = 0; j < rt.camera.yRes(); ++j)
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
    }

    QWidget::paintEvent(ev);
}
