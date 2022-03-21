QT       += core gui \
    xml \
    multimedia \
    multimediawidgets \
    charts \
    widgets \
    serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comportthread.cpp \
    imagetcpserver.cpp \
    main.cpp \
    newplayer.cpp \
    threadhandler.cpp \
    videosurface.cpp \
    videowidget.cpp

HEADERS += \
    comportthread.h \
    imagetcpserver.h \
    newplayer.h \
    threadhandler.h \
    videosurface.h \
    videowidget.h

FORMS += \
    newplayer.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
