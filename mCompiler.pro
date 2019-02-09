TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lexicalanalyser.cpp \
    token.cpp

HEADERS += \
    lexicalanalyser.h

DISTFILES += \
    input_file
