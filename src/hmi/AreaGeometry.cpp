#include "AreaGeometry.h"

#include <cmath>

void AreaGeometry::CircleGeometry::circleToPolygon(QPolygonF &destPolygon, const QPointF &centerPointWgs, double radiusDeg, const MapSettings & mapSettings)
{
    MapSettings settings = mapSettings;
    settings.zoom.setCurrentZoomLevel(ZoomInfo::MaxZoomLevel);

    QPointF centerPointPix(settings.getPixelForLon(centerPointWgs.x()), settings.getPixelForLat(centerPointWgs.y()));
    double radiusPix = settings.getPixelForLon((centerPointWgs.x()) + radiusDeg) - centerPointPix.x();

    for(double a = 0.; a <= 360.; a += 0.5)
    {
        QPointF point(sin(a * M_PI / 180.0) * radiusPix + centerPointPix.x(),
                      cos(a * M_PI / 180.0) * radiusPix + centerPointPix.y());

        point.setX(settings.getLonForPixelOld(point.x()));
        point.setY(settings.getLatForPixel(settings.windowPixelToMapPixelY(point.y())));

        destPolygon.push_back(point);
    }
}

void AreaGeometry::CircleGeometry::circleToPolygon(CircleGeometry &circleGeometry, const MapSettings &mapSettings)
{
    circleToPolygon(circleGeometry.polygonizedCircle, circleGeometry.centerPoint, circleGeometry.radius, mapSettings);
}
