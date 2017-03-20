# we are going to build a lib
TEMPLATE = lib
# this flag makes it a static .a lib easier for this simple program
CONFIG += staticlib
# use C++ 11
CONFIG += c++11
# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) { cache() }
QT += core

TARGET = BuildingLib
DESTDIR = $$PWD/lib
OBJECTS_DIR = $$PWD/obj

SOURCES=$$PWD/src/*
HEADERS+=$$PWD/include/*

macx:QMAKE_CXXFLAGS += -arch x86_64
QMAKE_CXXFLAGS += -msse -msse2 -msse3

LIBS += -L/usr/local/lib -lgtest -lpthread

INCLUDEPATH += include
INCLUDEPATH += /usr/local/include
INCLUDEPATH += $$PWD/../glm/
