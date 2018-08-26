#include "PolygonDownloaderPrepare.h"
#include "Layers/MapSettings.h"
#include "OSMTileDownloaderInfoWidget.h"
#include "DeleteOldMapsUtils.h"

PolygonDownloaderPrepare::PolygonDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
    : GeometryDownloaderPrepare(prepareSetup, parent)
{

}

void PolygonDownloaderPrepare::prepare()
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

        if(_prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Polygon &&
           _prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
           _prepareSetup._downloadSetup.geometry.geometry.canConvert<QPolygonF>() == true)
        {
            // FIXME: upravit algoritmus hladania dlazdic pre stiahnutie
            // tak aby sa stahovali dlazdice, ktore su obsiahnute vnutri polygonu
            // resp. do neho zasahuju. A nie cez obalku polygonu
            QPolygonF polygon = _prepareSetup._downloadSetup.geometry.geometry.value<QPolygonF>();

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

                        if(polygon.intersects(tilePolygon) == false)
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

                            _prepareSetup._downloader->addDownloadItem(item);
                        }
                    }
                }
            }
        }
    }
}
