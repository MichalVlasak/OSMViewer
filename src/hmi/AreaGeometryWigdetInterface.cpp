#include "AreaGeometryWigdetInterface.h"
#include "RectangleAreaWidget.h"
#include "PolygonAreaWidget.h"
#include "LineWidget.h"
#include "CircleWidget.h"

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
        return new PolygonAreaWidget(geometry, parent);
    }
    else if(geometry.geometryType == AreaGeometry::Type::Line &&
            geometry.geometry.isNull() == false &&
            geometry.geometry.canConvert<AreaGeometry::LineBufferGeometry>() == true)
    {
        return new LineWidget(geometry, parent);
    }
    else if(geometry.geometryType == AreaGeometry::Type::Circle &&
            geometry.geometry.isNull() == false &&
            geometry.geometry.canConvert<AreaGeometry::CircleGeometry>() == true)
    {
        return new CircleWidget(geometry, parent);
    }
    else
    {
        throw std::logic_error("Cannot detect right geometry type!");
    }

    return nullptr;
}
