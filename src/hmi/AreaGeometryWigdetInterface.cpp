#include "AreaGeometryWigdetInterface.h"
#include "RectangleAreaWidget.h"

AreaGeometryWigdetInterface::AreaGeometryWigdetInterface(QWidget * parent)
    : QWidget(parent)
{
}

AreaGeometryWigdetInterface::~AreaGeometryWigdetInterface()
{
}

AreaGeometryWigdetInterface * AreaGeometryWigdetInterface::createGeometryWidget(const AreaGeometry & geometry, QWidget * parent)
{
    if(geometry.geometryType == AreaGeometry::Type::Rectangle &&
       geometry.geometry.isNull() == false &&
       geometry.geometry.canConvert<QRectF>() == true)
    {
        return new RectangleAreaWidget(geometry, parent);
    }
    else if(geometry.geometryType == AreaGeometry::Type::Polygon &&
       geometry.geometry.isNull() == false &&
       geometry.geometry.canConvert<QPolygonF>() == true)
    {
        throw std::logic_error("Polygon is not yet implemented!");
        return nullptr;
    }
    else
    {
        throw std::logic_error("Cannot detect right geometry type!");
    }

    return nullptr;
}
