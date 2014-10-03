#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T23:44:54
#
#-------------------------------------------------

QT       -= gui

TARGET = arangodb
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11
MOC_DIR = moc
OBJECTS_DIR = obj

DEFINES += ARANGODB_LIBRARY

SOURCES +=  document.cpp \
    documentprivate.cpp \
    request.cpp \
    arangodata.cpp \
    collection.cpp \
    arangodb.cpp \
    arangodbprivate.cpp \
    aqlquery.cpp \
    aqlqueryprivate.cpp \
    collectionprivate.cpp

HEADERS +=  document.h \
    documentprivate.h \
    request.h \
    arangodata.h \
    collection.h \
    arango.h \
    arangodb_global.h \
    arangodb.h \
    arangodbprivate.h \
    aqlquery.h \
    aqlqueryprivate.h \
    collectionprivate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../syncrequest/release/ -lsyncrequest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../syncrequest/debug/ -lsyncrequest
else:unix:!macx: LIBS += -L$$OUT_PWD/../syncrequest/ -lsyncrequest

INCLUDEPATH += $$PWD/../syncrequest
DEPENDPATH += $$PWD/../syncrequest
