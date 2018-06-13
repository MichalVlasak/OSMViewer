#include "GpxInfoFileWidgetConfig.h"
#include "AppSettings.h"

bool GpxInfoFileWidgetConfig::CadentionShowMaxChck = false;
bool GpxInfoFileWidgetConfig::ElevationShowMaxChck = false;
bool GpxInfoFileWidgetConfig::HeartRateShowMaxChck = false;
bool GpxInfoFileWidgetConfig::TemperatureShowMaxChck = false;

bool GpxInfoFileWidgetConfig::CadentionShowGrapChck = true;
bool GpxInfoFileWidgetConfig::ElevationShowGraphChck = true;
bool GpxInfoFileWidgetConfig::HeartRateShowGraphChck = true;
bool GpxInfoFileWidgetConfig::TemperatureShowGraphChck = true;

GpxInfoFileWidgetConfig::TabWidgetTypeEnum GpxInfoFileWidgetConfig::TabWidgetType = GpxInfoFileWidgetConfig::TabWidgetTypeEnum::Table;

GpxInfoFileWidgetConfig::GpxInfoFileWidgetConfig()
{

}

void GpxInfoFileWidgetConfig::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement gpxInfoFileCfgElement = document.createElement("GpxInfoFileWidgetConfig");
    rootElement.appendChild(gpxInfoFileCfgElement);

    QDomElement cadentionShowMaxElement = document.createElement("CadentionShowMaxChck");
    gpxInfoFileCfgElement.appendChild(cadentionShowMaxElement);
    QDomText cadentionShowMaxText = document.createTextNode(QString::number(CadentionShowMaxChck));
    cadentionShowMaxElement.appendChild(cadentionShowMaxText);

    QDomElement cadentionShowGraphElement = document.createElement("CadentionShowGrapChck");
    gpxInfoFileCfgElement.appendChild(cadentionShowGraphElement);
    QDomText cadentionShowGraphText = document.createTextNode(QString::number(CadentionShowGrapChck));
    cadentionShowGraphElement.appendChild(cadentionShowGraphText);

    QDomElement elevationShowMaxElement = document.createElement("ElevationShowMaxChck");
    gpxInfoFileCfgElement.appendChild(elevationShowMaxElement);
    QDomText elevationShowMaxText = document.createTextNode(QString::number(ElevationShowMaxChck));
    elevationShowMaxElement.appendChild(elevationShowMaxText);

    QDomElement elevationShowGraphElement = document.createElement("ElevationShowGraphChck");
    gpxInfoFileCfgElement.appendChild(elevationShowGraphElement);
    QDomText elevationShowGraphText = document.createTextNode(QString::number(ElevationShowGraphChck));
    elevationShowGraphElement.appendChild(elevationShowGraphText);

    QDomElement heartRateShowMaxElement = document.createElement("HeartRateShowMaxChck");
    gpxInfoFileCfgElement.appendChild(heartRateShowMaxElement);
    QDomText heartRateShowMaxText = document.createTextNode(QString::number(HeartRateShowMaxChck));
    heartRateShowMaxElement.appendChild(heartRateShowMaxText);

    QDomElement heartRateShowGraphElement = document.createElement("HeartRateShowGraphChck");
    gpxInfoFileCfgElement.appendChild(heartRateShowGraphElement);
    QDomText heartRateShowGraphText = document.createTextNode(QString::number(HeartRateShowGraphChck));
    heartRateShowGraphElement.appendChild(heartRateShowGraphText);

    QDomElement temperatureShowMaxElement = document.createElement("TemperatureShowMaxChck");
    gpxInfoFileCfgElement.appendChild(temperatureShowMaxElement);
    QDomText temperatureShowMaxText = document.createTextNode(QString::number(TemperatureShowMaxChck));
    temperatureShowMaxElement.appendChild(temperatureShowMaxText);

    QDomElement temperatureShowGraphElement = document.createElement("TemperatureShowGraphChck");
    gpxInfoFileCfgElement.appendChild(temperatureShowGraphElement);
    QDomText temperatureShowGraphText = document.createTextNode(QString::number(TemperatureShowGraphChck));
    temperatureShowGraphElement.appendChild(temperatureShowGraphText);

    QDomElement tabWidgetTypeElement = document.createElement("TabWidgetType");
    gpxInfoFileCfgElement.appendChild(tabWidgetTypeElement);
    QDomText tabWidgetTypeText = document.createTextNode(QString::number(int(TabWidgetType)));
    tabWidgetTypeElement.appendChild(tabWidgetTypeText);
}

static bool getBoolFromString(const QString valueStr, bool & valueBool)
{
    if(valueStr.isEmpty() == false)
    {
        bool isOk = false;

        int num = valueStr.toInt(&isOk);

        if(isOk == true)
        {
            valueBool = (num > 0) ? true : false;

            return true;
        }
    }

    return false;
}

bool GpxInfoFileWidgetConfig::restoreConfig(QDomDocument &document)
{
    bool result = true;

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList gpxNodes = rootElem.elementsByTagName("GpxInfoFileWidgetConfig");

        for(int i = 0; i < gpxNodes.size(); i++)
        {
            QDomNode gpxNode = gpxNodes.at(i);

            if(gpxNode.isNull() == false)
            {
                QString value = AppSettings::getValueString(gpxNode, "CadentionShowMaxChck");

                result &= getBoolFromString(value, CadentionShowMaxChck);

                value = AppSettings::getValueString(gpxNode, "CadentionShowGrapChck");

                result &= getBoolFromString(value, CadentionShowGrapChck);

                value = AppSettings::getValueString(gpxNode, "ElevationShowMaxChck");

                result &= getBoolFromString(value, ElevationShowMaxChck);

                value = AppSettings::getValueString(gpxNode, "ElevationShowGraphChck");

                result &= getBoolFromString(value, ElevationShowGraphChck);

                value = AppSettings::getValueString(gpxNode, "HeartRateShowMaxChck");

                result &= getBoolFromString(value, HeartRateShowMaxChck);

                value = AppSettings::getValueString(gpxNode, "HeartRateShowGraphChck");

                result &= getBoolFromString(value, HeartRateShowGraphChck);

                value = AppSettings::getValueString(gpxNode, "TemperatureShowMaxChck");

                result &= getBoolFromString(value, TemperatureShowMaxChck);

                value = AppSettings::getValueString(gpxNode, "TemperatureShowGraphChck");

                result &= getBoolFromString(value, TemperatureShowGraphChck);

                value = AppSettings::getValueString(gpxNode, "TabWidgetType");

                if(value.isEmpty() == false)
                {
                    bool isOk = false;

                    int num = value.toInt(&isOk);

                    if(isOk == true)
                    {
                        TabWidgetType = TabWidgetTypeEnum(num);

                        result &= true;
                    }
                }
            }
        }
    }

    return result;
}
