#ifndef AREAGEOMETRY_H
#define AREAGEOMETRY_H

#include <QVariant>
#include <QPolygonF>
#include <QMetaType>

struct AreaGeometry
{
        enum class Type
        {
            Undefined, Rectangle, Polygon, Line
        };

        struct LineBufferGeometry
        {
                QPolygonF line;
                int bufferWidth = 0;
        };

        /**
         * @brief geometry - do geometrie je mozne vlozit nasledovne typy geometrii:
         * Rectangle - QRectF
         * Polygon - QPolygonF
         */
        QVariant geometry;
        Type geometryType = Type::Undefined;
};

Q_DECLARE_METATYPE(AreaGeometry::LineBufferGeometry)

#endif // AREAGEOMETRY_H
