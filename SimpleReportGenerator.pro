#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T15:55:42
#
#-------------------------------------------------

QT       += widgets

QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = SimpleReportGenerator
TEMPLATE = lib

DEFINES += SIMPLEREPORTGENERATOR_LIBRARY

SOURCES += SimpleReportGenerator.cpp \
    TabSet.cpp \
    TableWriter.cpp \
    SimpleReportViewer.cpp \
    TextStyle.cpp \
    TextStyleLib.cpp

HEADERS += SimpleReportGenerator.h\
        simplereportgenerator_global.h \
    TabSet.h \
    TableWriter.h \
    SimpleReportViewer.h \
    TextStyle.h \
    TextStyleLib.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SimpleReportViewer.ui
