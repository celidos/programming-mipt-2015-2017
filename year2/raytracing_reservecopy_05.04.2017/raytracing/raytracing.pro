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
    rtgeometry.cpp \
    paintdevice.cpp \
    primitive3d.cpp \
    rt_raytracer.cpp \
    rt_scene3d.cpp

HEADERS  += mainwindow.h \
    rtcamera3d.h \
    rtcolor.h \
    rtgeometry.h \
    rtprimitive3d_triangle.h \
    rtprimitive3d_sphere.h \
    rtprimitive3d.h \
    paintdevice.h \
    rtlighting.h \
    about.h \
    rt_raytracer.h \
    rt_scene3d.h

FORMS    += mainwindow.ui

LIBS += \
       -lboost_system\

DISTFILES += \
    ../build-raytracing-Desktop_Qt_5_7_0_GCC_64bit-Debug/scene01.rt


