TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=opengl

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/models
INCLUDEPATH += $$PWD/glm
INCLUDEPATH += $$PWD/Shaders

SOURCES += src/*
HEADERS += include/*

OTHER_FILES +=	glm/*\
glm/detail/* \
glm/gtc/* \
glm/gtx* \
glm/simd/* \
models/* \
models/Walls/* \
models/Corners/* \
Shaders/*

#macx
#{
#macx:LIBS += -F/Library/Frameworks -framework SDL2
#macx:INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
#macx:INCLUDEPATH += /usr/local/include/SDL2
#}

QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
LIBS += $$system(sdl2-config --libs)

LIBS += -L/usr/local/lib

macx:LIBS+= -framework OpenGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64
{
LIBS += -lGLEW
}
linux-clang
{
LIBS += -lGLEW
}



