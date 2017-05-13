QT += core
QT -= gui

CONFIG += c++11

TARGET = thread_pool
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    thread_pool.h
