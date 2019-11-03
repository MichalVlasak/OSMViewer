#ifndef AREAGEOMETRY_H
#define AREAGEOMETRY_H

#include <QVariant>
#include <QPolygonF>
#include <QMetaType>

#include "../Layers/MapSettings.h"

struct AreaGeometry
{
        enum class Type
        {
            Undefined, Rectangle, Polygon, Line, Circle
        };

        struct LineBufferGeometry
        {
                QPolygonF line;
                int bufferWidth = 0;
        };

        struct CircleGeometry
        {
                QPointF centerPoint;
                double radius = 0.;
                QPolygonF polygonizedCircle;

                /**
                 * @brief circleToPolygon Metoda sluzi na polygonizaciu kruhu. Aby sa definovany kruh stahoval ako
                 * kruh, tak definiciu kruhu spolygonizujem. Jednotlive body kruhu vypocitam v pixeloch (tym mam
                 * zarucene ze kruh bude kruhom) a nasledne ich prevediem do WGS suradnic. Ak by som polygonizoval
                 * kruh priamo vo WGS, tak ziskam elipsu, ktora cim viac bude na severe alebo juhu tym viac bude
                 * pretiahnuta
                 * @param circleGeometry Struktura definujuca kruh, kde suradnice stredu su vo WGS suradniciach
                 * a radius je v stupnoch v smere zemepisnej dlzky
                 * @param mapSettings Mapove nastavenia na prepocet WGS do pixelov a spat.
                 */
                static void circleToPolygon(CircleGeometry & circleGeometry, const MapSettings & mapSettings);

                /**
                 * @brief circleToPolygon vid popis metody
                 * circleToPolygon(CircleGeometry & circleGeometry, const MapSettings & mapSettings)
                 * @param destPolygon
                 * @param centerPointPix
                 * @param radiusPix
                 * @param mapSettings
                 */
                static void circleToPolygon(QPolygonF & destPolygon, const QPointF & centerPointPix, double radiusPix, const MapSettings & mapSettings);
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
Q_DECLARE_METATYPE(AreaGeometry::CircleGeometry)

#endif // AREAGEOMETRY_H
