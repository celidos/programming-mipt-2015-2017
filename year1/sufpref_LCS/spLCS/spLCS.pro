QT += core
QT -= gui

TARGET = spLCS
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++0x

