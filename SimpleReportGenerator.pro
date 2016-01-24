#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T15:55:42
#
#-------------------------------------------------

QT       += widgets

QMAKE_CXXFLAGS += -std=c++14

# Optimization level O3 for release
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3

# Optimization level O1 for debug
#QMAKE_CXXFLAGS_DEBUG += -O1

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = SimpleReportGenerator
TEMPLATE = lib

VERSION = 0.2.0

DEFINES += SIMPLEREPORTGENERATOR_LIBRARY

SOURCES += SimpleReportGenerator.cpp \
    TabSet.cpp \
    TableWriter.cpp \
    SimpleReportViewer.cpp \
    TextStyle.cpp \
    TextStyleLib.cpp \
    ReportGraphicsView.cpp \
    LineChart.cpp

HEADERS += SimpleReportGenerator.h\
        simplereportgenerator_global.h \
    TabSet.h \
    TableWriter.h \
    SimpleReportViewer.h \
    TextStyle.h \
    TextStyleLib.h \
    ReportGraphicsView.h \
    LineChart.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SimpleReportViewer.ui
