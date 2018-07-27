#include "OSMTileDownloaderPrepare.h"
#include "GeometryDownloaderPrepare.h"

#include "Layers/DownloadAreaHighlight.h"

#include <iostream>

OSMTileDownloaderPrepare::OSMTileDownloaderPrepare(OSMTileDownloader * downloader, OSMTileDownloaderInfoWidget * infoWidget, QObject *parent)
    : QThread(parent),
      _downloader(downloader),
      _infoWidget(infoWidget)
{
    QObject::connect(this, SIGNAL(columnIsPrepared()), _downloader, SLOT(startDownload()));
    QObject::connect(_infoWidget, SIGNAL(cancelDownloading()), SLOT(cancelDownloading()));
}

void OSMTileDownloaderPrepare::setDownloadAreaHighlight(DownloadAreaHighlight * downloadAreaHighlight)
{
    _downloadAreaHighlight = downloadAreaHighlight;
}

void OSMTileDownloaderPrepare::setDownloadParameters(OSMDownloadAreaDialog::Setup setup, const QString & tilesPath)
{
    _setup = setup;
    _tilesPath = tilesPath;
    _runPrepare = true;

    _infoWidget->setLevelRange(_setup.levelFrom, _setup.levelTo);

    if(_downloadAreaHighlight != nullptr)
    {
        _downloadAreaHighlight->setDownloadParams(setup);
    }

    start();
}

void OSMTileDownloaderPrepare::run()
{
    GeometryDownloaderPrepare::DownloaderPrepareSetup prepareSetup(_setup, _runPrepare, _downloader, _infoWidget, _tilesPath);

    GeometryDownloaderPrepare * prepare = GeometryDownloaderPrepare::createGeometryDownloaderPrepare(prepareSetup, this);

    QObject::connect(prepare, SIGNAL(columnIsPrepared()), SIGNAL(columnIsPrepared()));

    prepare->prepare();

    if(_downloadAreaHighlight != nullptr)
    {
        while(_downloader->isRunning() == true)
        {
            QThread::msleep(100);
        }

        emit allIsDownloaded();
    }
}

void OSMTileDownloaderPrepare::cancelDownloading()
{
    _runPrepare = false;
}
