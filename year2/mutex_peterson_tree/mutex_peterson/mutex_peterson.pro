QT -= gui

TARGET = mutex_peterson
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    tree_mutex.h

