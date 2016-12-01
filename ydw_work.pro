#-------------------------------------------------
#
# Project created by QtCreator 2016-03-01T16:29:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ydw_work
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mypixmapitem.cpp \
    imageprocesser.cpp \
    dlgcontraststretch.cpp \
    sixdim.cpp \
    dlgspatialfilter.cpp \
    dlgmedianfilter.cpp \
    dlgtransform.cpp\
    matrix_.cpp \
    dlgguassianblur.cpp \
    dlgedgeextraction.cpp

HEADERS  += mainwindow.h \
    mypixmapitem.h \
    imageprocesser.h \
    dlgcontraststretch.h \
    sixdim.h \
    dlgspatialfilter.h \
    dlgmedianfilter.h \
    dlgtransform.h\
    matrix_.h \
    dlgguassianblur.h \
    dlgedgeextraction.h

FORMS    += mainwindow.ui \
    dlgcontraststretch.ui \
    dlgspatialfilter.ui \
    dlgtransform.ui\
    dlgmedianfilter.ui \
    dlgguassianblur.ui \
    dlgedgeextraction.ui
