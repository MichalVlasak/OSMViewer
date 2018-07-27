#ifndef OSMTILEDOWNLOADERPREPARE_H
#define OSMTILEDOWNLOADERPREPARE_H

#include <QThread>

#include "OSMDownloadAreaDialog.h"
#include "OSMTileDownloader.h"
#include "OSMTileDownloaderInfoWidget.h"

class OSMTileDownloaderPrepare : public QThread
{
        Q_OBJECT
    public:
        explicit OSMTileDownloaderPrepare(OSMTileDownloader * downloader, OSMTileDownloaderInfoWidget * infoWidget, QObject *parent = 0);

    public:
        void setDownloadParameters(OSMDownloadAreaDialog::Setup setup, const QString & tilesPath);
        void setDownloadAreaHighlight(class DownloadAreaHighlight * downloadAreaHighlight);

    signals:
        void columnIsPrepared();
        void allIsDownloaded();

    private:
        void run();

    private slots:
        void cancelDownloading();

    private:
        OSMDownloadAreaDialog::Setup _setup;
        OSMTileDownloader * _downloader;
        OSMTileDownloaderInfoWidget * _infoWidget;
        QString _tilesPath;
        bool _runPrepare = true;
        class DownloadAreaHighlight * _downloadAreaHighlight = nullptr;
};

#endif // OSMTILEDOWNLOADERPREPARE_H
