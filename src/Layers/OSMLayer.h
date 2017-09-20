#ifndef OSMLAYER_H
#define OSMLAYER_H

#include "BaseLayer.h"
#include "src/DeleteOldMapsWidget.h"

class OSMLayer : public BaseLayer
{
    public:
        explicit OSMLayer(MapSettings & mapSettings);

        static const int TileSize = 256;
        static const QString DefaultMapDirectory;

    public:
        void paintEvent(QPaintEvent * paintEvent);
        void setOSMDirectorypath(QString dirPath);
        QString getOSMDirectorypath();
        void setOSMTileDownloader(class OSMTileDownloader * downloader);
        void setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings);

    private:
        QString _tilesPath;
        class OSMTileDownloader * _downloader = nullptr;
        DeleteOldMapsWidget::DeleteSettings _settings;
};

#endif // OSMLAYER_H
