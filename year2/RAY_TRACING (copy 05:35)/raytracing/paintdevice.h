#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QPicture>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class PaintDevice : public QWidget
{
    Q_OBJECT
public:
    PaintDevice(QWidget* parent = 0);
    virtual void paintEvent(QPaintEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

    bool showrect;

    QPicture *pict;
    QPainter *painter;

    QLabel * lbl;
public slots:
    void draw(bool x);
};

#endif // PAINTDEVICE_H
