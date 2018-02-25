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
    src/OSMTileDownloaderSetupWidget.cpp \
    src/AboutDialog.cpp \
    src/Layers/DownloadAreaHighlight.cpp \
    src/DeleteOldMapsWidget.cpp \
    src/DeleteOldMapsDialog.cpp \
    src/DeleteOldMapsUtils.cpp \
    src/CenterPointsManager.cpp \
    src/CenterPointsWidget.cpp \
    src/CenterPointEditDialog.cpp \
    src/WgsConversion.cpp \
    src/GroupEditDialog.cpp \
    src/OSMDownloadProjectWidget.cpp \
    src/OSMDownloadProjectModel.cpp \
    src/ProjectNameDialog.cpp \
    src/StoreConfigInterface.cpp

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
    src/OSMTileDownloaderSetupWidget.h \
    src/AboutDialog.h \
    src/Layers/DownloadAreaHighlight.h \
    src/AppVersion.h \
    src/DeleteOldMapsWidget.h \
    src/DeleteOldMapsDialog.h \
    src/DeleteOldMapsUtils.h \
    src/CenterPointStruct.h \
    src/CenterPointsManager.h \
    src/CenterPointsWidget.h \
    src/CenterPointEditDialog.h \
    src/WgsConversion.h \
    src/GroupEditDialog.h \
    src/OSMDownloadProjectWidget.h \
    src/OSMDownloadProjectModel.h \
    src/ProjectNameDialog.h \
    src/StoreConfigInterface.h

FORMS    += resources/MainWindow.ui \
    resources/OSMDirectoryPathDialog.ui \
    resources/OSMDownloadAreaDialog.ui \
    resources/OSMTileDownloaderInfoWidget.ui \
    resources/OSMTileDownloaderSetupWidget.ui \
    resources/AboutDialog.ui \
    resources/DeleteOldMapsWidget.ui \
    resources/DeleteOldMapsDialog.ui \
    resources/CenterPointsWidget.ui \
    resources/CenterPointEditDialog.ui \
    resources/GroupEditDialog.ui \
    resources/OSMDownloadProjectWidget.ui \
    resources/ProjectNameDialog.ui

RESOURCES += \
    resources/resources.qrc

SUBDIRS += \
    UnitTest.pro
