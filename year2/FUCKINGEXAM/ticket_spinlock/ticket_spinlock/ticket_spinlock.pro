QT += core
QT -= gui

CONFIG += c++11

TARGET = ticket_spinlock
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    ticket_spinlock.h
