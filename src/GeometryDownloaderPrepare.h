#ifndef GEOMETRYDOWNLOADERPREPARE_H
#define GEOMETRYDOWNLOADERPREPARE_H

#include "OSMDownloadAreaDialog.h"
#include "OSMTileDownloader.h"
#include "OSMTileDownloaderInfoWidget.h"

#include <QObject>

class GeometryDownloaderPrepare : public QObject
{
        Q_OBJECT

    public:
        struct DownloaderPrepareSetup
        {
                DownloaderPrepareSetup(const OSMDownloadAreaDialog::Setup & setup, bool & runPrepare, OSMTileDownloader * downloader, OSMTileDownloaderInfoWidget * infoWidget, const QString & tilesPath)
                    : _downloadSetup(setup),
                      _runPrepare(runPrepare),
                      _downloader(downloader),
                      _infoWidget(infoWidget),
                      _tilesPath(tilesPath)
                {
                }

                const OSMDownloadAreaDialog::Setup & _downloadSetup;
                bool & _runPrepare;
                OSMTileDownloader * _downloader;
                OSMTileDownloaderInfoWidget * _infoWidget;
                const QString & _tilesPath;
        };

    public:
        GeometryDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent = nullptr);
        virtual ~ GeometryDownloaderPrepare() = default;

        static GeometryDownloaderPrepare * createGeometryDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent);

        virtual void prepare() = 0;

    protected:
        const DownloaderPrepareSetup & _prepareSetup;
};

#endif // GEOMETRYDOWNLOADERPREPARE_H
