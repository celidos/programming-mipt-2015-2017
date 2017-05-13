QT += core
QT -= gui

TARGET = splay
CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

SOURCES += main.cpp \
    splaytree.cpp \
    naivearray.cpp

HEADERS += \
    splaytree.h \
    naivearray.h \
    array_interface.h \
    array_tester.h

