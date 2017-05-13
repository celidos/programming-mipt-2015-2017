QT += core
QT -= gui

TARGET = torrent_client
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11

LIBS += -lcurlpp
LIBS += -lcurl

TEMPLATE = app

SOURCES += \
    main.cpp \
    cpp-bencoding-master/src/Utils.cpp \
    cpp-bencoding-master/src/PrettyPrinter.cpp \
    cpp-bencoding-master/src/Encoder.cpp \
    cpp-bencoding-master/src/Decoder.cpp \
    cpp-bencoding-master/src/BString.cpp \
    cpp-bencoding-master/src/BList.cpp \
    cpp-bencoding-master/src/BItemVisitor.cpp \
    cpp-bencoding-master/src/BItem.cpp \
    cpp-bencoding-master/src/BInteger.cpp \
    cpp-bencoding-master/src/BDictionary.cpp

HEADERS += \
    cpp-bencoding-master/include/Utils.h \
    cpp-bencoding-master/include/PrettyPrinter.h \
    cpp-bencoding-master/include/Encoder.h \
    cpp-bencoding-master/include/Decoder.h \
    cpp-bencoding-master/include/BString.h \
    cpp-bencoding-master/include/BList.h \
    cpp-bencoding-master/include/BItemVisitor.h \
    cpp-bencoding-master/include/BItem.h \
    cpp-bencoding-master/include/BInteger.h \
    cpp-bencoding-master/include/bencoding.h \
    cpp-bencoding-master/include/BDictionary.h

DISTFILES +=

