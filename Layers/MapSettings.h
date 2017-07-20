#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QWidget>

#include "ZoomInfo.h"

class MapSettings
{
    public:
        MapSettings();

        double getPixelForLat(double lat);
        double getPixelForLon(double lon);
        double getLonForPixelOld(double pix);
        double getLonForPixel(double pix);
        double getLatForPixel(double pix);

        int windowPixelToMapPixelX(int pix);
        int windowPixelToMapPixelY(int pix);

        static int long2tilex(double lon, int z);
        static int lat2tiley(double lat, int z);
        static double tilex2long(int x, int z);
        static double tiley2lat(int y, int z);
        static double tiley2lat2(int y, int z);

    public:
        QPointF worldCenter;
        QPointF oldCenter;
        QPointF movingStartPoint;
        QWidget * widget = nullptr;
        ZoomInfo zoom;
};

#endif // MAPSETTINGS_H
