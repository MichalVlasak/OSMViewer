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
            // FIXME: upravit algoritmus hladania dlazdic pre stiahnutie
            // tak aby sa stahovali dlazdice, ktore pretinaju definovanu ciaru,
            // resp. su v definovanom okoli ciary. tym sa usetri velke mnozstvo
            // testovania pre dlazdice ktore niesu v blyzkosti danej ciary, ale
            // spadaju do ohranicenia pre definovanu ciaru
            AreaGeometry::LineBufferGeometry lineGeometry = _prepareSetup._downloadSetup.geometry.geometry.value<AreaGeometry::LineBufferGeometry>();
            QPolygonF polygon = lineGeometry.line;

            QRectF geometryRect = polygon.boundingRect();

            double lonFrom = geometryRect.topLeft().x();
            double latFrom = geometryRect.topLeft().y();
            double lonTo = geometryRect.bottomRight().x();
            double latTo = geometryRect.bottomRight().y();

            if(latFrom < latTo)
            {
                std::swap(latFrom, latTo);
            }

            int tileColStart = MapSettings::long2tilex(lonFrom, level);
            int tileColStop = MapSettings::long2tilex(lonTo, level);

            int tileRowStart = MapSettings::lat2tiley(latFrom, level);
            int tileRowStop = MapSettings::lat2tiley(latTo, level);

            OSMTileDownloaderInfoWidget::LevelInfo levelInfo;

            levelInfo.colFrom = tileColStart;
            levelInfo.colTo = tileColStop;
            levelInfo.rowFrom = tileRowStart;
            levelInfo.rowTo = tileRowStop;

            _prepareSetup._infoWidget->setLevelInfo(level, levelInfo);

            for(int col = tileColStart; col <= tileColStop; col++)
            {
                if(_prepareSetup._runPrepare == false)
                {
                    break;
                }

                QString columnStr = QString::number(col);

                for(int row = tileRowStart; row <= tileRowStop; row++)
                {
                    if(_prepareSetup._runPrepare == false)
                    {
                        break;
                    }

                    if((col + 1) < maxCol && (row + 1) < maxRow)
                    {
                        QPointF topLeft(MapSettings::tilex2long(col, level), MapSettings::tiley2lat(row, level));
                        QPointF topRight(MapSettings::tilex2long(col + 1, level), MapSettings::tiley2lat(row, level));
                        QPointF bottomLeft(MapSettings::tilex2long(col, level), MapSettings::tiley2lat(row + 1, level));
                        QPointF bottomRight(MapSettings::tilex2long(col + 1, level), MapSettings::tiley2lat(row + 1, level));

                        QPolygonF tilePolygon;

                        tilePolygon.push_back(topLeft);
                        tilePolygon.push_back(topRight);
                        tilePolygon.push_back(bottomLeft);
                        tilePolygon.push_back(bottomRight);
                        tilePolygon.push_back(topLeft);

                        bool intersected = false;

                        // Nemozem testovat pretnutie polygonu samotneho, ale musim ist
                        // po jednotlivych useckach polygonu a tie testovat. Ak by som
                        // siel po polygone ako takom tak by sa mi stahovalo vsetko
                        // co je v danom polygone, tj. ciary
                        for(int i = 0; i < polygon.size() - 1; i++)
                        {
                            QPolygonF tmp;

                            tmp.push_back(polygon[i]);
                            tmp.push_back(polygon[i + 1]);

                            intersected |= tmp.intersects(tilePolygon);
                        }

                        if(intersected == false)
                        {
                            continue;
                        }
                    }

                    QString filePath = _prepareSetup._tilesPath + levelStr + "/" + columnStr + "/" + QString::number(row) + ".png";
                    QFile file(filePath);

                    DeleteOldMapsUtils::tryDeleteFile(filePath, _prepareSetup._downloadSetup.deleteSettings, true);

                    if(file.exists() == false)
                    {
                        if(_prepareSetup._downloader != nullptr)
                        {
                            //std::cout << "prepare - " << filePath.toStdString() << " - Dont exist!" << std::endl;

                            OSMTileDownloader::DownloadItem item;

                            item.column = col;
                            item.level = level;
                            item.row = row;
                            item.fullPath = filePath;
                            item.basePath = _prepareSetup._tilesPath;

                            while(_prepareSetup._downloader->isFreeQueue() == false)
                            {
                                QThread::msleep(500);
                            }

                            _prepareSetup._downloader->addUrlToDownload(item, false);
                        }
                    }

                    if(_prepareSetup._runPrepare == true)
                    {
                        emit columnIsPrepared();
                    }

                    for(int colBuf = 0 - lineGeometry.bufferWidth; colBuf <=lineGeometry.bufferWidth; colBuf++)
                    {
                        QString columnStr = QString::number(col + colBuf);

                        for(int rowBuf = 0 - lineGeometry.bufferWidth; rowBuf <=lineGeometry.bufferWidth; rowBuf++)
                        {
                            if(colBuf == 0 && rowBuf == 0)
                            {
                                continue;
                            }

                            QString filePath = _prepareSetup._tilesPath + levelStr + "/" + columnStr + "/" + QString::number(row + rowBuf) + ".png";
                            QFile file(filePath);

                            DeleteOldMapsUtils::tryDeleteFile(filePath, _prepareSetup._downloadSetup.deleteSettings, true);

                            if(file.exists() == false)
                            {
                                if(_prepareSetup._downloader != nullptr)
                                {
                                    //std::cout << "prepare - " << filePath.toStdString() << " - Dont exist!" << std::endl;

                                    OSMTileDownloader::DownloadItem item;

                                    item.column = col + colBuf;
                                    item.level = level;
                                    item.row = row + rowBuf;
                                    item.fullPath = filePath;
                                    item.basePath = _prepareSetup._tilesPath;

                                    while(_prepareSetup._downloader->isFreeQueue() == false)
                                    {
                                        QThread::msleep(500);
                                    }

                                    _prepareSetup._downloader->addUrlToDownload(item, false);
                                }
                            }

                            if(_prepareSetup._runPrepare == true)
                            {
                                emit columnIsPrepared();
                            }
                        }
                    }
                }
            }
        }
    }
}
