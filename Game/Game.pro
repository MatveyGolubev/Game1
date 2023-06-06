TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    map.cpp \
    drawpers.cpp \
    start.cpp \
    animation.cpp \
    knife.cpp \
    interface.cpp

#LIBS+=-lglut -lGL -lGLU # linux libs # without qt =)

#INCLUDEPATH += C:\Qt\freeglut\include
INCLUDEPATH += _PRO_FILE_PWD_\Gl

#LIBS += -LC:\Qt\freeglut\lib  \
LIBS += -L$$_PRO_FILE_PWD_\lib  \
    -lfreeglut \
   -lopengl32   # windows libs

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    map.h \
    drawpers.h \
    interface.h \
    logick.h \
    start.h \
    animation.h \
    knife.h \
    structures.h




