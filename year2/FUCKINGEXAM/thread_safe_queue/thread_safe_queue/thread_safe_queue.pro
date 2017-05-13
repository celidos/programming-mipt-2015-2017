QT += core
QT -= gui

CONFIG += c++11

TARGET = thread_safe_queue
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    thread_safe_queue.h
