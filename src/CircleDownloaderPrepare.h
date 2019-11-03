#ifndef CIRCLEDOWNLOADERPREPARE_H
#define CIRCLEDOWNLOADERPREPARE_H

#include "PolygonDownloaderPrepare.h"

class CircleDownloaderPrepare : public PolygonDownloaderPrepare
{
    public:
        CircleDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent = nullptr);
        virtual ~CircleDownloaderPrepare() = default;

        void prepare();
};

#endif // CIRCLEDOWNLOADERPREPARE_H
