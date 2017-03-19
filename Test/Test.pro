TARGET = gtest
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=$$PWD/src/*.cpp
HEADERS +=$$PWD/include/*.h
OBJECTS_DIR = $$PWD/obj

INCLUDEPATH+= /usr/local/include
LIBS+= -L/usr/local/lib -lgtest -lpthread

INCLUDEPATH += $$PWD/../lib/BuildingLib/include
LIBS+= -L $$PWD/../lib/BuildingLib/lib -lBuildingLib

INCLUDEPATH += $$PWD/../lib/glm/
macx:CONFIG-=app_bundle

OTHER_FILES +=	../models/*
