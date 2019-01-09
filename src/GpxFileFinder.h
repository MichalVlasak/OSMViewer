#ifndef GPXFILEFINDER_H
#define GPXFILEFINDER_H

#include "hmi/AreaGeometry.h"
#include "GpxManager.h"

class GpxFileFinder
{
    public:
        GpxFileFinder(GpxManagerPtr gpxManager);

        void find(const AreaGeometry & geometry) const;

    private:
        GpxManagerPtr _gpxManager;
};

#endif // GPXFILEFINDER_H
