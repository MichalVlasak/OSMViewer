#include "LineDownloaderPrepare.h"
#include "Layers/MapSettings.h"
#include "OSMTileDownloaderInfoWidget.h"
#include "DeleteOldMapsUtils.h"

LineDownloaderPrepare::LineDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
    : GeometryDownloaderPrepare(prepareSetup, parent)
{

}

void LineDownloaderPrepare::prepare()
{
    for(int level = _prepareSetup._downloadSetup.levelFrom; level <= _prepareSetup._downloadSetup.levelTo; level++)
    {
        if(_prepareSetup._runPrepare == false)
        {
            break;
        }

        QString levelStr = QString::number(level);

        int maxCol = pow(2., level);
        int maxRow = pow(2., level);

        if(_prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Line &&
           _prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
           _prepareSetup._downloadSetup.geometry.geometry.canConvert<AreaGeometry::LineBufferGeometry>() == true)
        {
            AreaGeometry::LineBufferGeometry lineGeometry = _prepareSetup._downloadSetup.geometry.geometry.value<AreaGeometry::LineBufferGeometry>();
            QPolygonF polygon = lineGeometry.line;

            for(int i = 0; i < polygon.size() - 1; i++)
            {
                QPointF startPoint = polygon[i];
                QPointF stopPoint = polygon[i + 1];
                QLineF line(startPoint, stopPoint);

                int tileStartCol = MapSettings::long2tilex(startPoint.y(), level);
                int tileStartRow = MapSettings::lat2tiley(startPoint.x(), level);
                int tileStopCol = MapSettings::long2tilex(stopPoint.y(), level);
                int tileStopRow = MapSettings::lat2tiley(stopPoint.x(), level);

                double maxTileDiff = std::max(std::fabs(tileStartCol - tileStopCol), std::fabs(tileStartRow - tileStopRow));

                if(maxTileDiff == 0.)
                {
                    maxTileDiff = 1.;
                }

                maxTileDiff *= 10.;

                double koef = 1. / maxTileDiff;
                int tileColPrev = 0;
                int tileRowPrev = 0;

                for(double i = 0.;  i <= (1. + koef); i += koef)
                {
                    QPointF currentPoint = line.pointAt(i);

                    int tileCol = MapSettings::long2tilex(currentPoint.x(), level);
                    int tileRow = MapSettings::lat2tiley(currentPoint.y(), level);

                    if(i == 0.)
                    {
                        tileColPrev = tileCol;
                        tileRowPrev = tileRow;
                    }
                    else
                    {
                        if(tileColPrev == tileCol && tileRowPrev == tileRow)
                        {
                            continue;
                        }

                        tileColPrev = tileCol;
                        tileRowPrev = tileRow;
                    }

                    QString columnStr = QString::number(tileCol);

                    QString filePath = _prepareSetup._tilesPath + levelStr + "/" + columnStr + "/" + QString::number(tileRow) + ".png";
                    QFile file(filePath);

                    DeleteOldMapsUtils::tryDeleteFile(filePath, _prepareSetup._downloadSetup.deleteSettings, true);

                    if(file.exists() == false)
                    {
                        if(_prepareSetup._downloader != nullptr)
                        {
                            //std::cout << "prepare - " << filePath.toStdString() << " - Dont exist!" << std::endl;

                            OSMTileDownloader::DownloadItem item;

                            item.column = tileCol;
                            item.level = level;
                            item.row = tileRow;
                            item.fullPath = filePath;
                            item.basePath = _prepareSetup._tilesPath;

                            while(_prepareSetup._downloader->isFreeQueue() == false)
                            {
                                QThread::msleep(500);
                            }

                            _prepareSetup._downloader->addDownloadItem(item);
                        }
                    }

                    for(int colBuf = 0 - lineGeometry.bufferWidth; colBuf <=lineGeometry.bufferWidth; colBuf++)
                    {
                        if((tileCol + colBuf) > maxCol)
                        {
                            break;
                        }

                        QString columnStr = QString::number(tileCol + colBuf);

                        for(int rowBuf = 0 - lineGeometry.bufferWidth; rowBuf <=lineGeometry.bufferWidth; rowBuf++)
                        {
                            if((tileRow + rowBuf) > maxRow)
                            {
                                break;
                            }

                            if(colBuf == 0 && rowBuf == 0)
                            {
                                continue;
                            }

                            QString filePath = _prepareSetup._tilesPath + levelStr + "/" + columnStr + "/" + QString::number(tileRow + rowBuf) + ".png";
                            QFile file(filePath);

                            DeleteOldMapsUtils::tryDeleteFile(filePath, _prepareSetup._downloadSetup.deleteSettings, true);

                            if(file.exists() == false)
                            {
                                if(_prepareSetup._downloader != nullptr)
                                {
                                    //std::cout << "prepare - " << filePath.toStdString() << " - Dont exist!" << std::endl;

                                    OSMTileDownloader::DownloadItem item;

                                    item.column = tileCol + colBuf;
                                    item.level = level;
                                    item.row = tileRow + rowBuf;
                                    item.fullPath = filePath;
                                    item.basePath = _prepareSetup._tilesPath;

                                    while(_prepareSetup._downloader->isFreeQueue() == false)
                                    {
                                        QThread::msleep(500);
                                    }

                                    _prepareSetup._downloader->addDownloadItem(item);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
