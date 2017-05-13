QT += core
QT -= gui

CONFIG += c++11

TARGET = cyclic_barrier
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    barrier.h
