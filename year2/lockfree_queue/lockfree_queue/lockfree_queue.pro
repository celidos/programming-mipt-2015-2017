QT += core
QT -= gui

CONFIG += c++11

TARGET = lockfree_queue
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    lock_free_queue_old.h \
    lock_free_queue.h
