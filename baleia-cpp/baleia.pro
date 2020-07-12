TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/cli_inputs.cpp \
        src/librpidata.cpp \
        src/main.cpp

HEADERS += \
    includes/cli_inputs.h \
    includes/librpidata.h
