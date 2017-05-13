#include "paintdevice.h"

#include "rt_raytracer.h"

PaintDevice::PaintDevice(QWidget *parent): QWidget(parent)
{
    //setStyleSheet("background: white");
    showrect = false;
}

void PaintDevice::draw(bool x)
{
    pict = new QPicture;
    painter =  new QPainter(pict);

    // --------------------

    RayTracer rt;
    //rt.readFromRtFile("input4.txt");
    //rt.readFromRtFile("scene01.rt");
    //rt.readFromRtFile("input246.txt");
    rt.readFromRtFile("input007.txt");
    rt.antiAliasingXrate = rt.antiAliasingYrate = 1;
    //"scene01.rt");
    Color **bitmap = rt.render();

    // --------------------

    for (uint i = 0; i < rt.camera.xRes(); ++i)
    {
        for (uint j = 0; j < rt.camera.yRes(); ++j)
        {
            painter->setPen(QColor::fromRgbF(bitmap[i][j].r, bitmap[i][j].g, bitmap[i][j].b));
            //painter->setPen(Qt::blue);//QColor::fromRgbF(i /600.0, j/600.0, (i + j)/.700));
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

void PaintDevice::mouseMoveEvent(QMouseEvent *ev)
{
    lbl->setText(QString::number(ev->x()) + " x " + QString::number(ev->y()));
}
