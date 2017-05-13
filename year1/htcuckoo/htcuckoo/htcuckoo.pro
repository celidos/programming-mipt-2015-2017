QT += core
QT -= gui

TARGET = htcuckoo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hashtable_cuckoo.cpp

HEADERS += \
    hashtable_cuckoo.h

