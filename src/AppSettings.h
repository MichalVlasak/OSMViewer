#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "Layers/MapSettings.h"

#include <QtXml>

class AppSettings
{
    public:
        AppSettings();
        ~AppSettings();

        static QString getValueString(QDomNode & node, const QString & name);

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

        void storeProjects(class OSMDownloadProjectModel * projectsModel);
        bool restoreProjects(class OSMDownloadProjectModel * projectsModel);

    private:
        QString _settingsFileName;
        QDomDocument _doc;
        QDomElement _rootElement;
};

#endif // APPSETTINGS_H
