#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "raytracer.h"
#include "paintdevice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PaintDevice *pd;

public slots:
    void raytrace(bool x);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
