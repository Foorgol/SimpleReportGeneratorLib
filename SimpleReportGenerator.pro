#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T15:55:42
#
#-------------------------------------------------

QT       += widgets printsupport svg

TARGET = SimpleReportGenerator

CONFIG(debug, debug|release) {
  TARGET = SimpleReportGeneratord
}

TEMPLATE = lib

CONFIG += c++14 debug_and_release dll build_all

VERSION = 0.3.1

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
        #simplereportgenerator_global.h \
    TabSet.h \
    TableWriter.h \
    SimpleReportViewer.h \
    TextStyle.h \
    TextStyleLib.h \
    ReportGraphicsView.h \
    LineChart.h

!unix {
    target.path = D:/msys64/usr/local/lib
    header_files.path = D:/msys64/usr/local/include/SimpleReportGeneratorLib
    header_files.files = $$HEADERS
    INSTALLS += header_files
}

unix {
    target.path = /usr/local/lib
    header_files.path = /usr/local/include/SimpleReportGeneratorLib
    header_files.files = $$HEADERS
    INSTALLS += header_files
}

INSTALLS += target

FORMS += \
    SimpleReportViewer.ui
