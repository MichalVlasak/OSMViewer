#include "GpxInfoFileWidgetConfig.h"
#include "AppSettings.h"

bool GpxInfoFileWidgetConfig::CadentionCheckChck = false;
bool GpxInfoFileWidgetConfig::ElevationCheckChck = false;
bool GpxInfoFileWidgetConfig::HeartRateCheckChck = false;
bool GpxInfoFileWidgetConfig::TemperatureCheckChck = false;

GpxInfoFileWidgetConfig::GpxInfoFileWidgetConfig()
{

}

void GpxInfoFileWidgetConfig::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement gpxInfoFileCfgElement = document.createElement("GpxInfoFileWidgetConfig");
    rootElement.appendChild(gpxInfoFileCfgElement);

    QDomElement cadentionElement = document.createElement("CadentionCheckChck");
    gpxInfoFileCfgElement.appendChild(cadentionElement);
    QDomText cadentionText = document.createTextNode(QString::number(CadentionCheckChck));
    cadentionElement.appendChild(cadentionText);

    QDomElement elevationElement = document.createElement("ElevationCheckChck");
    gpxInfoFileCfgElement.appendChild(elevationElement);
    QDomText elevationText = document.createTextNode(QString::number(ElevationCheckChck));
    elevationElement.appendChild(elevationText);

    QDomElement heartRateElement = document.createElement("HeartRateCheckChck");
    gpxInfoFileCfgElement.appendChild(heartRateElement);
    QDomText heartRateText = document.createTextNode(QString::number(HeartRateCheckChck));
    heartRateElement.appendChild(heartRateText);

    QDomElement temperatureElement = document.createElement("TemperatureCheckChck");
    gpxInfoFileCfgElement.appendChild(temperatureElement);
    QDomText temperatureText = document.createTextNode(QString::number(TemperatureCheckChck));
    temperatureElement.appendChild(temperatureText);
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
                QString value = AppSettings::getValueString(gpxNode, "CadentionCheckChck");

                result &= getBoolFromString(value, CadentionCheckChck);

                value = AppSettings::getValueString(gpxNode, "ElevationCheckChck");

                result &= getBoolFromString(value, ElevationCheckChck);

                value = AppSettings::getValueString(gpxNode, "HeartRateCheckChck");

                result &= getBoolFromString(value, HeartRateCheckChck);

                value = AppSettings::getValueString(gpxNode, "TemperatureCheckChck");

                result &= getBoolFromString(value, TemperatureCheckChck);
            }
        }
    }

    return result;
}
