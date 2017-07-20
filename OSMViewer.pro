#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T20:48:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = OSMViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/Layers/BaseLayer.cpp \
    src/Layers/OSMLayer.cpp \
    src/Layers/GridLayer.cpp \
    src/Layers/ZoomInfo.cpp \
    src/Layers/PaintingWidget.cpp \
    src/Layers/MapSettings.cpp \
    src/AppSettings.cpp \
    src/OSMDirectoryPathDialog.cpp \
    src/MapContextMenu.cpp \
    src/OSMDownloadAreaDialog.cpp \
    src/OSMTileDownloader.cpp \
    src/OSMTileDownloaderInfoWidget.cpp \
    src/OSMTileDownloaderPrepare.cpp \
    src/OSMTileDownloaderSetupWidget.cpp

HEADERS  += src/MainWindow.h \
    src/Layers/BaseLayer.h \
    src/Layers/OSMLayer.h \
    src/Layers/GridLayer.h \
    src/Layers/ZoomInfo.h \
    src/Layers/PaintingWidget.h \
    src/Layers/MapSettings.h \
    src/AppSettings.h \
    src/OSMDirectoryPathDialog.h \
    src/MapContextMenu.h \
    src/OSMDownloadAreaDialog.h \
    src/OSMTileDownloader.h \
    src/OSMTileDownloaderInfoWidget.h \
    src/OSMTileDownloaderPrepare.h \
    src/OSMTileDownloaderSetupWidget.h

FORMS    += resources/MainWindow.ui \
    resources/OSMDirectoryPathDialog.ui \
    resources/OSMDownloadAreaDialog.ui \
    resources/OSMTileDownloaderInfoWidget.ui \
    resources/OSMTileDownloaderSetupWidget.ui

RESOURCES += \
    resources/resources.qrc
