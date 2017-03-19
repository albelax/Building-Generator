TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=opengl

OBJECTS_DIR = ./obj
INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/models
INCLUDEPATH += $$PWD/../glm/
INCLUDEPATH += $$PWD/Shaders

SOURCES += src/*
HEADERS += include/*

OTHER_FILES +=	models/* \
								models/Walls/* \
								models/Corners/* \
								Shaders/*


QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
LIBS += $$system(sdl2-config --libs)
INCLUDEPATH += /usr/local/include/SDL2

INCLUDEPATH+= /usr/local/include
LIBS+= -L/usr/local/lib -lgtest -lpthread

INCLUDEPATH += $$PWD/../BuildingLib/include
LIBS+= -L $$PWD/../BuildingLib/lib -lBuildingLib

macx:LIBS += -framework OpenGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64
{
LIBS += -lGLEW
}
linux-clang
{
LIBS += -lGLEW
}



