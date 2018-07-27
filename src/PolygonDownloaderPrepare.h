#ifndef POLYGONDOWNLOADERPREPARE_H
#define POLYGONDOWNLOADERPREPARE_H

#include "GeometryDownloaderPrepare.h"

class PolygonDownloaderPrepare : public GeometryDownloaderPrepare
{
    public:
        PolygonDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent = nullptr);
        virtual ~PolygonDownloaderPrepare() = default;

        void prepare();
};

#endif // POLYGONDOWNLOADERPREPARE_H
