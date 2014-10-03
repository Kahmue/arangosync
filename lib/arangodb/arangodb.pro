#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T23:44:54
#
#-------------------------------------------------

QT       -= gui

TARGET = arangodb
TEMPLATE = lib

DEFINES += ARANGODB_LIBRARY

SOURCES += arangodb.cpp

HEADERS += arangodb.h\
        arangodb_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
