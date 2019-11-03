#include "RectangleDownloaderPrepare.h"
#include "Layers/MapSettings.h"
#include "OSMTileDownloaderInfoWidget.h"
#include "DeleteOldMapsUtils.h"

#include <QRectF>

RectangleDownloaderPrepare::RectangleDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
    : GeometryDownloaderPrepare(prepareSetup, parent)
{
}

void RectangleDownloaderPrepare::prepare()
{
    for(int level = _prepareSetup._downloadSetup.levelFrom; level <= _prepareSetup._downloadSetup.levelTo; level++)
    {
        if(_prepareSetup._runPrepare == false)
        {
            break;
        }

        QString levelStr = QString::number(level);

        if(_prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Rectangle &&
           _prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
           _prepareSetup._downloadSetup.geometry.geometry.canConvert<QRectF>() == true)
        {
            QRectF geometryRect = _prepareSetup._downloadSetup.geometry.geometry.toRectF();

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
