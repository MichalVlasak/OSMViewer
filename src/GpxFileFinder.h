#ifndef GPXFILEFINDER_H
#define GPXFILEFINDER_H

#include "hmi/AreaGeometry.h"
#include "GpxManager.h"

class GpxFileFinder
{
    public:
        GpxFileFinder(GpxManager * gpxManager);

        void find(const AreaGeometry & geometry) const;

    private:
        GpxManager * _gpxManager = nullptr;
};

#endif // GPXFILEFINDER_H
