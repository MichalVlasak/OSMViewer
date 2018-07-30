#ifndef AREAGEOMETRY_H
#define AREAGEOMETRY_H

#include <QVariant>

struct AreaGeometry
{
        enum class Type
        {
            Undefined, Rectangle, Polygon, Line
        };

        /**
         * @brief geometry - do geometrie je mozne vlozit nasledovne typy geometrii:
         * Rectangle - QRectF
         * Polygon - QPolygonF
         */
        QVariant geometry;
        Type geometryType = Type::Undefined;
};

#endif // AREAGEOMETRY_H
