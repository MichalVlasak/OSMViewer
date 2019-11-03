#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QWidget>

#include "ZoomInfo.h"
#include "src/StoreConfigInterface.h"

class MapSettings : public StoreConfigInterface
{
    public:
        MapSettings();
        MapSettings(const MapSettings & settings);

        double getPixelForLat(double lat) const;
        double getPixelForLon(double lon) const;
        double getLonForPixelOld(double pix) const;
        double getLonForPixel(double pix) const;
        double getLatForPixel(double pix) const;

        int windowPixelToMapPixelX(int pix) const;
        int windowPixelToMapPixelY(int pix) const;

        static int long2tilex(double lon, int z);
        static int lat2tiley(double lat, int z);
        static double tilex2long(int x, int z);
        static double tiley2lat(int y, int z);
        static double tiley2lat2(int y, int z);

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public:
        QPointF worldCenter;
        QPointF oldCenter;
        QPointF movingStartPoint;
        QWidget * widget = nullptr;
        ZoomInfo zoom;
};

#endif // MAPSETTINGS_H
