#ifndef SELECTGEOMETRY_H
#define SELECTGEOMETRY_H

#include <QVariant>

struct SelectGeometry
{
        enum class Type
        {
            Undefined, Rectangle, Polygon
        };

        /**
         * @brief geometry - do geometrie je mozne vlozit nasledovne typy geometrii:
         * Rectangle - QRectF
         * Polygon - QPolygonF
         */
        QVariant geometry;
        Type geometryType = Type::Undefined;
};

#endif // SELECTGEOMETRY_H
