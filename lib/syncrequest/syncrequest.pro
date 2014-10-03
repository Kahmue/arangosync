#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T11:24:20
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = syncrequest
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

DEFINES += SYNCREQUEST_LIBRARY

MOC_DIR = moc
OBJECTS_DIR = obj

SOURCES += syncrequest.cpp \
    net.cpp \
    syncrequestprivate.cpp

HEADERS += syncrequest.h\
        syncrequest_global.h \
    net.h \
    syncrequestprivate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
