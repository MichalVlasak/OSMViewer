#include "OSMTileDownloaderPrepare.h"

#include "Layers/MapSettings.h"

#include <iostream>

OSMTileDownloaderPrepare::OSMTileDownloaderPrepare(OSMTileDownloader * downloader, OSMTileDownloaderInfoWidget * infoWidget, QObject *parent)
    : QThread(parent),
      _downloader(downloader),
      _infoWidget(infoWidget)
{
    QObject::connect(this, SIGNAL(columnIsPrepared()), _downloader, SLOT(startDownload()));
    QObject::connect(_infoWidget, SIGNAL(cancelDownloading()), SLOT(cancelDownloading()));
}

void OSMTileDownloaderPrepare::setDownloadParameters(OSMDownloadAreaDialog::Setup setup, const QString & tilesPath)
{
    _setup = setup;
    _tilesPath = tilesPath;
    _runPrepare = true;

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

        int tileColStart = MapSettings::long2tilex(_setup.lonFrom, level);
        int tileColStop = MapSettings::long2tilex(_setup.lonTo, level);

        int tileRowStart = MapSettings::lat2tiley(_setup.latFrom, level);
        int tileRowStop = MapSettings::lat2tiley(_setup.latTo, level);

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

                        _downloader->addUrlToDownload(item, false);
                    }
                }
            }

            if(_runPrepare == true)
            {
                emit columnIsPrepared();
            }
        }
    }
}

void OSMTileDownloaderPrepare::cancelDownloading()
{
    _runPrepare = false;
}
