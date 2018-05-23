#ifndef OSMLAYER_H
#define OSMLAYER_H

#include "BaseLayer.h"
#include "src/DeleteOldMapsWidget.h"
#include "src/StoreConfigInterface.h"

class OSMLayer : public BaseLayer, public StoreConfigInterface
{
    public:
        explicit OSMLayer(MapSettings & mapSettings);

        static const int TileSize = 256;
        static const QString DefaultMapDirectory;

    public:
        void paintEvent(QPainter & painter);
        void setOSMDirectorypath(QString dirPath);
        QString getOSMDirectorypath();
        void setOSMTileDownloader(class OSMTileDownloader * downloader);
        void setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings);

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    private:
        QString _tilesPath;
        class OSMTileDownloader * _downloader = nullptr;
        DeleteOldMapsWidget::DeleteSettings _settings;
};

#endif // OSMLAYER_H
