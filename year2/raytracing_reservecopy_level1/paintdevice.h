#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QPicture>
#include <QPainter>
#include <QWidget>

class PaintDevice : public QWidget
{
    Q_OBJECT
public:
    PaintDevice(QWidget* parent = 0);
    virtual void paintEvent(QPaintEvent *ev);

    bool showrect;

    QPicture *pict;
    QPainter *painter;
public slots:
    void draw();
};

#endif // PAINTDEVICE_H
