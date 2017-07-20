#ifndef OSMLAYER_H
#define OSMLAYER_H

#include "BaseLayer.h"

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

    private:
        QString _tilesPath;
        class OSMTileDownloader * _downloader = nullptr;
};

#endif // OSMLAYER_H
