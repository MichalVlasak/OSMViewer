#include "OSMLayer.h"
#include "src/OSMTileDownloader.h"
#include "src/DeleteOldMapsUtils.h"

#include <QPainter>
#include <QWidget>

#include <iostream>
#include <cmath>

const QString OSMLayer::DefaultMapDirectory = "./tile.openstreetmap.org/";

OSMLayer::OSMLayer(MapSettings & mapSettings)
    : BaseLayer(mapSettings)
{
    //_tilesPath = "/opt/rsys/var/cache/tile.openstreetmap.org/";
    _tilesPath = DefaultMapDirectory;
}

void OSMLayer::setOSMDirectorypath(QString dirPath)
{
    _tilesPath = dirPath;

    if(_tilesPath.at(_tilesPath.size() - 1) != '/')
    {
        _tilesPath += "/";
    }
}

QString OSMLayer::getOSMDirectorypath()
{
    return _tilesPath;
}

void OSMLayer::setOSMTileDownloader(OSMTileDownloader *downloader)
{
    _downloader = downloader;
}

void OSMLayer::paintEvent(QPaintEvent *paintEvent)
{
    //std::cout << "paint" << std::endl;
    //std::cout << "paintEvent center " << _mapSettings.center.x() << " " << _mapSettings.center.y() << std::endl;

    QPainter painter(_mapSettings.widget);

    unsigned level = _mapSettings.zoom.getCurrentZoomLevel();
    unsigned count = pow(2., level);
    float offset = float(count) / 2.;
    QString levelStr = QString::number(level);

    int wHeight = _mapSettings.widget->height();
    int wWidth = _mapSettings.widget->width();

    int colStart = (0 - _mapSettings.worldCenter.x() + (offset * TileSize)) / TileSize;
    colStart = (colStart < 0) ? 0 : colStart;

    for(unsigned column = colStart; column < count; column++)
    {
        QString columnStr = QString::number(column);

        int pPosX = _mapSettings.worldCenter.x() + (column - offset) * TileSize;

        if(pPosX > (wWidth + 256))
        {
            continue;
        }

        int rowStart = (0 - _mapSettings.worldCenter.y() + (offset * TileSize)) / TileSize;
        rowStart = (rowStart < 0) ? 0 : rowStart;

        for(unsigned row = rowStart; row < count; row++)
        {
            int pPosY = _mapSettings.worldCenter.y() + (row - offset) * TileSize;

            if(pPosY > (wHeight + 256))
            {
                continue;
            }

            QString filePath = _tilesPath + levelStr + "/" + columnStr + "/" + QString::number(row) + ".png";

            if(_downloader != nullptr && _downloader->isDownloadingEnable() == true)
            {
                DeleteOldMapsUtils::tryDeleteFile(filePath, _settings, false);
            }

            QPixmap pixmap(filePath);

            if(pixmap.isNull() == false)
            {
                painter.drawPixmap(pPosX, pPosY, TileSize, TileSize, pixmap);
            }
            else
            {
                if(_downloader != nullptr && _downloader->isDownloadingEnable() == true)
                {
                    //std::cout << filePath.toStdString() << " - Dont exist!" << std::endl;

                    OSMTileDownloader::DownloadItem item;

                    item.column = column;
                    item.level = level;
                    item.row = row;
                    item.fullPath = filePath;
                    item.basePath = _tilesPath;

                    _downloader->addUrlToDownload(item);
                }
            }
        }
    }
}

void OSMLayer::setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings)
{
    _settings = settings;
}
