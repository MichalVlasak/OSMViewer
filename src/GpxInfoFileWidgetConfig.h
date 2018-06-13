#ifndef GPXINFOFILEWIDGETCONFIG_H
#define GPXINFOFILEWIDGETCONFIG_H

#include "StoreConfigInterface.h"

class GpxInfoFileWidgetConfig : public StoreConfigInterface
{
    public:
        GpxInfoFileWidgetConfig();
        virtual ~GpxInfoFileWidgetConfig() = default;

        enum class TabWidgetTypeEnum
        {
            Table, Graph
        };

        static bool CadentionShowMaxChck;
        static bool ElevationShowMaxChck;
        static bool HeartRateShowMaxChck;
        static bool TemperatureShowMaxChck;

        static bool CadentionShowGrapChck;
        static bool ElevationShowGraphChck;
        static bool HeartRateShowGraphChck;
        static bool TemperatureShowGraphChck;

        static TabWidgetTypeEnum TabWidgetType;

    public:
        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);
};

#endif // GPXINFOFILEWIDGETCONFIG_H
