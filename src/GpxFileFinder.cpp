#include "GpxFileFinder.h"
#include "GpxFilesListWidget.h"
#include "MainWindow.h"

GpxFileFinder::GpxFileFinder(GpxManager * gpxManager)
    : _gpxManager(gpxManager)
{
}

void GpxFileFinder::find(const AreaGeometry &geometry) const
{
    if(_gpxManager != nullptr)
    {
        QPolygonF polygon;
        bool isInitialized = false;

        if(geometry.geometryType == AreaGeometry::Type::Rectangle &&
           geometry.geometry.isNull() == false &&
           geometry.geometry.canConvert<QRectF>() == true)
        {
            QRectF geometryRect = geometry.geometry.toRectF();

            polygon = QPolygonF(geometryRect);

            isInitialized = true;
        }
        else if(geometry.geometryType == AreaGeometry::Type::Polygon &&
                geometry.geometry.isNull() == false &&
                geometry.geometry.canConvert<QPolygonF>() == true)
        {
            polygon = geometry.geometry.value<QPolygonF>();

            isInitialized = true;
        }

        if(isInitialized == true)
        {
            const GpxManager::GpxVector & allGpxs = _gpxManager->getGpxVector();
            bool isSomeFinded = false;

            for(const GpxManager::GpxItem & item : allGpxs)
            {
                bool isFinded = false;

                for(const GpxManager::Point & point : item.pointVector)
                {
                    QPointF qPoint(point.lon, point.lat);
                    QPolygonF tmpPoly;

                    tmpPoly.push_back(qPoint);

                    if(polygon.intersects(tmpPoly) == true)
                    {
                        isFinded = true;

                        break;
                    }
                }

                GpxManager::GpxItem * itemNoConst = const_cast<GpxManager::GpxItem*>(&item);

                if(itemNoConst != nullptr)
                {
                    itemNoConst->highlight = isFinded;
                    isSomeFinded |= isFinded;
                }
            }

            MainWindow * mainWin = MainWindow::getInstance();

            if(mainWin != nullptr)
            {
                GpxFilesListWidget * gpxFileListWidget = mainWin->getGpxFilesListWidget();

                if(gpxFileListWidget != nullptr)
                {
                    gpxFileListWidget->setEnabledClearHighlightedButton(isSomeFinded);
                }
            }
        }
    }
}
