#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T15:45:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raytracing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    raytracer.cpp \
    rtgeometry.cpp \
    paintdevice.cpp

HEADERS  += mainwindow.h \
    raytracer.h \
    rtcamera3d.h \
    rtcolor.h \
    rtgeometry.h \
    rtprimitive3d_triangle.h \
    rtprimitive3d_sphere.h \
    rtprimitive3d.h \
    rtray_dir.h \
    rtscene3d.h \
    paintdevice.h \
    rtlighting.h

FORMS    += mainwindow.ui

LIBS += \
       -lboost_system\


