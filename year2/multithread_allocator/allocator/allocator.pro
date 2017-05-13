QT += core
QT -= gui

CONFIG += c++11

TARGET = allocator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    mtallocator.h
