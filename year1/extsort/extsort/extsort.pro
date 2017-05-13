QT += core
QT -= gui

TARGET = extsort
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    extsort.cpp

HEADERS += \
    extsort.h \
    mergesort.h \
    heapsort_mod.h

