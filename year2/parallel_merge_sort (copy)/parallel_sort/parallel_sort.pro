QT += core
QT -= gui

CONFIG += c++11

TARGET = parallel_sort
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    threadpool.h \
    myparallel_sort.h
