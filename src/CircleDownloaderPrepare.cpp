#include "CircleDownloaderPrepare.h"
#include "Layers/MapSettings.h"
#include "OSMTileDownloaderInfoWidget.h"
#include "DeleteOldMapsUtils.h"

CircleDownloaderPrepare::CircleDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent)
    : PolygonDownloaderPrepare(prepareSetup, parent)
{
}

void CircleDownloaderPrepare::prepare()
{
    for(int level = _prepareSetup._downloadSetup.levelFrom; level <= _prepareSetup._downloadSetup.levelTo; level++)
    {
        if(_prepareSetup._runPrepare == false)
        {
            break;
        }

        int maxCol = pow(2., level);
        int maxRow = pow(2., level);

        if(_prepareSetup._downloadSetup.geometry.geometryType == AreaGeometry::Type::Circle &&
           _prepareSetup._downloadSetup.geometry.geometry.isNull() == false &&
           _prepareSetup._downloadSetup.geometry.geometry.canConvert<AreaGeometry::CircleGeometry>() == true)
        {
            AreaGeometry::CircleGeometry circleGeometry = _prepareSetup._downloadSetup.geometry.geometry.value<AreaGeometry::CircleGeometry>();

            preparePolygon(circleGeometry.polygonizedCircle, maxCol, maxRow, level);
        }
    }
}
