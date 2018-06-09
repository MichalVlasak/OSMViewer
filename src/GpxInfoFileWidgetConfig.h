#ifndef GPXINFOFILEWIDGETCONFIG_H
#define GPXINFOFILEWIDGETCONFIG_H

#include "StoreConfigInterface.h"

class GpxInfoFileWidgetConfig : public StoreConfigInterface
{
    public:
        GpxInfoFileWidgetConfig();
        virtual ~GpxInfoFileWidgetConfig() = default;

        static bool CadentionCheckChck;
        static bool ElevationCheckChck;
        static bool HeartRateCheckChck;
        static bool TemperatureCheckChck;

    public:
        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);
};

#endif // GPXINFOFILEWIDGETCONFIG_H
