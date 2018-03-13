#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T19:53:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# TARGET = GraphicsScene
TARGET = DelaunayVisual
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS_RELEASE -= -O1

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += main.cpp\
        MainWindow.cpp \
    MyGraphicsView.cpp \
    gNode.cpp \
    gEdge.cpp \
    algorithms/delaunay.cpp \
    algorithms/triangle.cpp \
    algorithms/oldmain.cpp \
    gCircle.cpp

HEADERS  += MainWindow.hpp \
    MyGraphicsView.hpp \
    gNode.hpp \
    gEdge.hpp \
    algorithms/delaunay.hpp \
    algorithms/edge.hpp \
    algorithms/triangle.hpp \
    algorithms/vector2.hpp \
    gCircle.hpp \
    VisualisationColors.hpp

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    algorithms/oldmakefile_delemelater
