QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_tests_checkheadersfornesting.cpp \
            ../tocGen/tocgen.cpp

HEADERS += ../tocGen/tocgen.h
