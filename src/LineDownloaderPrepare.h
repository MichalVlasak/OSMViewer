#ifndef LINEDOWNLOADERPREPARE_H
#define LINEDOWNLOADERPREPARE_H

#include "GeometryDownloaderPrepare.h"

class LineDownloaderPrepare : public GeometryDownloaderPrepare
{
    public:
        LineDownloaderPrepare(const DownloaderPrepareSetup & prepareSetup, QObject * parent = nullptr);
        virtual ~LineDownloaderPrepare() = default;

        void prepare();
};

#endif // LINEDOWNLOADERPREPARE_H
