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

        static bool CadentionCheckChck;
        static bool ElevationCheckChck;
        static bool HeartRateCheckChck;
        static bool TemperatureCheckChck;
        static TabWidgetTypeEnum TabWidgetType;

    public:
        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);
};

#endif // GPXINFOFILEWIDGETCONFIG_H
