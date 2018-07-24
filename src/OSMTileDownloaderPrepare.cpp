#include "OSMTileDownloaderPrepare.h"
#include "DeleteOldMapsUtils.h"

#include "Layers/MapSettings.h"
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
    for(int level = _setup.levelFrom; level <= _setup.levelTo; level++)
    {
        if(_runPrepare == false)
        {
            break;
        }

        QString levelStr = QString::number(level);

        if(_setup.geometry.geometryType == SelectGeometry::Type::Rectangle &&
           _setup.geometry.geometry.isNull() == false &&
           _setup.geometry.geometry.canConvert<QRectF>() == true)
        {
            QRectF geometryRect = _setup.geometry.geometry.toRectF();

            double lonFrom = geometryRect.topLeft().x();
            double latFrom = geometryRect.topLeft().y();
            double lonTo = geometryRect.bottomRight().x();
            double latTo = geometryRect.bottomRight().y();

            int tileColStart = MapSettings::long2tilex(lonFrom, level);
            int tileColStop = MapSettings::long2tilex(lonTo, level);

            int tileRowStart = MapSettings::lat2tiley(latFrom, level);
            int tileRowStop = MapSettings::lat2tiley(latTo, level);

            OSMTileDownloaderInfoWidget::LevelInfo levelInfo;

            levelInfo.colFrom = tileColStart;
            levelInfo.colTo = tileColStop;
            levelInfo.rowFrom = tileRowStart;
            levelInfo.rowTo = tileRowStop;

            _infoWidget->setLevelInfo(level, levelInfo);

            for(int col = tileColStart; col <= tileColStop; col++)
            {
                if(_runPrepare == false)
                {
                    break;
                }

                QString columnStr = QString::number(col);

                for(int row = tileRowStart; row <= tileRowStop; row++)
                {
                    if(_runPrepare == false)
                    {
                        break;
                    }

                    QString filePath = _tilesPath + levelStr + "/" + columnStr + "/" + QString::number(row) + ".png";
                    QFile file(filePath);

                    DeleteOldMapsUtils::tryDeleteFile(filePath, _setup.deleteSettings, true);

                    if(file.exists() == false)
                    {
                        if(_downloader != nullptr)
                        {
                            //std::cout << "prepare - " << filePath.toStdString() << " - Dont exist!" << std::endl;

                            OSMTileDownloader::DownloadItem item;

                            item.column = col;
                            item.level = level;
                            item.row = row;
                            item.fullPath = filePath;
                            item.basePath = _tilesPath;

                            while(_downloader->isFreeQueue() == false)
                            {
                                QThread::msleep(500);
                            }

                            _downloader->addUrlToDownload(item, false);
                        }
                    }

                    if(_runPrepare == true)
                    {
                        emit columnIsPrepared();
                    }
                }
            }
        }
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

void OSMTileDownloaderPrepare::cancelDownloading()
{
    _runPrepare = false;
}
