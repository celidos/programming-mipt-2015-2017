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
    paintdevice.cpp \
    rt_raytracer.cpp \
    rt_scene3d.cpp \
    rt_primitive3d.cpp \
    rt_geometry.cpp \
    rt_material.cpp \
    rt_lighting.cpp

HEADERS  += mainwindow.h \
    paintdevice.h \
    rt_raytracer.h \
    rt_scene3d.h \
    rt_material.h \
    rt_color.h \
    rt_geometry.h \
    rt_camera3d.h \
    rt_lighting.h \
    rt_primitive3d.h \
    rt_primitive3d_sphere.h \
    rt_primitive3d_triangle.h \
    rt_about.h \
    debug_functional.h

FORMS    += mainwindow.ui

LIBS += \
       -lboost_system\

DISTFILES += \
    ../build-raytracing-Desktop_Qt_5_7_0_GCC_64bit-Debug/scene01.rt \
    ../build-raytracing-Desktop_Qt_5_7_0_GCC_64bit-Debug/input4.txt


