#include "GeometryDownloaderPrepare.h"
#include "RectangleDownloaderPrepare.h"
#include "PolygonDownloaderPrepare.h"
#include "LineDownloaderPrepare.h"

GeometryDownloaderPrepare::GeometryDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
    : QObject(parent),
      _prepareSetup(prepareSetup)
{

}

GeometryDownloaderPrepare * GeometryDownloaderPrepare::createGeometryDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
{
    if(prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Rectangle &&
       prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
       prepareSetup._downloadSetup.geometry.geometry.canConvert<QRectF>() == true)
    {
        return new RectangleDownloaderPrepare(prepareSetup, parent);
    }
    else if(prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Polygon &&
            prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
            prepareSetup._downloadSetup.geometry.geometry.canConvert<QPolygonF>() == true)
    {
        return new PolygonDownloaderPrepare(prepareSetup, parent);
    }
    else if(prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Line &&
            prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
            prepareSetup._downloadSetup.geometry.geometry.canConvert<AreaGeometry::LineBufferGeometry>() == true)
    {
        return new LineDownloaderPrepare(prepareSetup, parent);
    }
    else
    {
        throw std::logic_error("Cannot detect right geometry type!");
    }

    return nullptr;
}
