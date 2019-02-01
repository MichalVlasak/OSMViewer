#include "OSMTileDownloaderPrepare.h"

#include "Layers/DownloadAreaHighlight.h"

#include <iostream>
#include <QMessageBox>

OSMTileDownloaderPrepare::OSMTileDownloaderPrepare(OSMTileDownloader * downloader, OSMTileDownloaderInfoWidget * infoWidget, QObject *parent)
    : QThread(parent),
      _downloader(downloader),
      _infoWidget(infoWidget)
{
    QObject::connect(_infoWidget, SIGNAL(cancelDownloading()), SLOT(cancelDownloading()));
}

OSMTileDownloaderPrepare::~OSMTileDownloaderPrepare()
{
    if(_prepareSetup != nullptr)
    {
        delete _prepareSetup;
        _prepareSetup = nullptr;
    }
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

    if(_prepareSetup != nullptr)
    {
        delete _prepareSetup;
        _prepareSetup = nullptr;
    }

    _prepareSetup = new GeometryDownloaderPrepare::DownloaderPrepareSetup(_setup, _runPrepare, _downloader, _infoWidget, _tilesPath);

    try
    {
        _prepareImpl = GeometryDownloaderPrepare::createGeometryDownloaderPrepare(*_prepareSetup, this);
    }
    catch(std::exception & ex)
    {
        QMessageBox::warning(_infoWidget, tr("Error"), tr("Error by preparing download:\n") + QString::fromStdString(std::string(ex.what())));
    }

    start();
}

void OSMTileDownloaderPrepare::run()
{
    try
    {
        if(_prepareImpl != nullptr)
        {
            _prepareImpl->prepare();
        }

        // priprava prebehla, nastavenie mozem vymazat
        if(_prepareSetup != nullptr)
        {
            delete _prepareSetup;
            _prepareSetup = nullptr;
        }

        if(_downloadAreaHighlight != nullptr)
        {
            while(_downloader->isRunning() == true)
            {
                QThread::msleep(100);
            }

            emit allIsDownloaded();
        }
    }
    catch(std::exception & ex)
    {
        QMessageBox::warning(_infoWidget, tr("Error"), tr("Error by preparing download:\n") + QString::fromStdString(std::string(ex.what())));
    }
}

void OSMTileDownloaderPrepare::cancelDownloading()
{
    _runPrepare = false;
}
