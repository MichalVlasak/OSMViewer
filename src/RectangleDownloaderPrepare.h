#ifndef RECTANGLEDOWNLOADERPREPARE_H
#define RECTANGLEDOWNLOADERPREPARE_H

#include "GeometryDownloaderPrepare.h"

class RectangleDownloaderPrepare : public GeometryDownloaderPrepare
{
    public:
        RectangleDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent = nullptr);
        virtual ~RectangleDownloaderPrepare() = default;

        void prepare();
};

#endif // RECTANGLEDOWNLOADERPREPARE_H
