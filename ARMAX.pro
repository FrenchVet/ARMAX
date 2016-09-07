#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T10:15:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ARMAX
TEMPLATE = app


SOURCES += main.cpp\
        armaxmainwindow.cpp \
    qcustomplot.cpp \
    other_functions.cpp \
    csvparser.cc \
    armaxmodel.cpp \
    armaxsolver.cpp \
    customplotdialog.cpp \
    fittingthread.cpp

HEADERS  += armaxmainwindow.h \
    qcustomplot.h \
    other_functions.h \
    csvparser.h \
    dlib/optimization.h \
    armaxmodel.h \
    armaxsolver.h \
    armaxfunctor.h \
    customplotdialog.h \
    fittingthread.h


FORMS    += armaxmainwindow.ui \
    customplotdialog.ui

