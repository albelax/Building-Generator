# we are going to build a lib
TEMPLATE=lib
# this flag makes it a static .a lib easier for this simple program
CONFIG+=staticlib
# use C++ 11
CONFIG+=c++11
# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) { cache() }
QT += core

INCLUDEPATH += $$PWD/../glm/
#DEPENDPATH += $$PWD/../glm/

TARGET=BuildingLib
DESTDIR=$$PWD/lib
OBJECTS_DIR=$$PWD/obj

SOURCES=$$PWD/src/*
HEADERS+=$$PWD/include/*

# add the ngl lib
# this is where to look for includes
INCLUDEPATH +=include

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64

QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
LIBS += $$system(sdl2-config --libs)

LIBS += -L/usr/local/lib
macx:LIBS+= -framework OpenGL
# now if we are under unix and not on a Mac (i.e. linux) define GLEW

