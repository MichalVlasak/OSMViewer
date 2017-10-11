#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "Layers/MapSettings.h"

#include <QtXml>

class AppSettings
{
    public:
        AppSettings();
        ~AppSettings();

        void storeMainWindowSettings(class MainWindow * mainWindow);
        bool restoreMainWindowSettings(class MainWindow * mainWindow);

        void storeOSMDirectoryPath(class OSMLayer * layer);
        bool restoreOSMDirectoryPath(class OSMLayer * layer);

        void storeMapSettings(MapSettings & mapSettings);
        bool restoreMapSettings(MapSettings & mapSettings);

        void storeDownloadSettings(class OSMTileDownloader * downloader);
        bool restoreDownloadSettings(class OSMTileDownloader * downloader);

        void storeDownloadAreaHighlightSettings(class DownloadAreaHighlight * highlighter);
        bool restoreDownloadAreaHighlightSettings(class DownloadAreaHighlight * highlighter);

        void storeCenterPoints(class CenterPointsManager * pointsManager);
        bool restoreCenterPoints(class CenterPointsManager * pointsManager);

    private:
        QString _settingsFileName;
        QDomDocument _doc;
        QDomElement _rootElement;
};

#endif // APPSETTINGS_H
