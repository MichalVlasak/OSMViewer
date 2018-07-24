#include "DownloadAreaHighlight.h"
#include "../AppSettings.h"

#include <QPainter>
#include <QTimer>

DownloadAreaHighlight::DownloadAreaHighlight(MapSettings & mapSettings, QObject * parent)
    : QObject(parent),
      BaseLayer(mapSettings)
{
    clearDownloadParams();
}

void DownloadAreaHighlight::paintEvent(QPainter & painter)
{
    if(_isDownloadParamsSetupEnable == true && _isVisible == true)
    {
        if(_setup.geometry.geometryType == SelectGeometry::Type::Rectangle &&
           _setup.geometry.geometry.isNull() == false &&
           _setup.geometry.geometry.canConvert<QRectF>() == true)
        {
            QRectF geometryRect = _setup.geometry.geometry.toRectF();

            double lonFrom = geometryRect.topLeft().x();
            double latFrom = geometryRect.topLeft().y();
            double lonTo = geometryRect.bottomRight().x();
            double latTo = geometryRect.bottomRight().y();

            QPoint startPointSelectArea(_mapSettings.getPixelForLon(lonFrom), _mapSettings.getPixelForLat(latFrom));
            QPoint endPointSelectArea(_mapSettings.getPixelForLon(lonTo), _mapSettings.getPixelForLat(latTo));

            QRect rect = QRect(startPointSelectArea, endPointSelectArea);

            painter.setPen(QPen(QColor(255, 0, 0, 50), 1));
            painter.setBrush(QColor(255, 0, 0, 50));
            painter.drawRect(rect);
        }
    }
}

void DownloadAreaHighlight::setDownloadParams(const OSMDownloadAreaDialog::Setup & setup)
{
    _setup = setup;
    _isDownloadParamsSetupEnable = true;
}

void DownloadAreaHighlight::resetDownloadParams()
{
    clearDownloadParams();

    QTimer::singleShot(1000, _mapSettings.widget, SLOT(repaint()));
}

void DownloadAreaHighlight::clearDownloadParams()
{
    _setup.geometry.geometryType = SelectGeometry::Type::Undefined;
    _setup.geometry.geometry.clear();
    _setup.levelFrom = 0.;
    _setup.levelTo = 0.;
    _isDownloadParamsSetupEnable = false;
}

void DownloadAreaHighlight::setVisible(bool value)
{
    _isVisible = value;

    QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
}

bool DownloadAreaHighlight::isVisible()
{
    return _isVisible;
}

void DownloadAreaHighlight::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement downloaderElement = document.createElement("DownloadAreaHighlight");
    rootElement.appendChild(downloaderElement);

    QDomElement downloadEnableElement = document.createElement("VisibleDownloadAreaHighlight");
    downloaderElement.appendChild(downloadEnableElement);
    QDomText downloadEnableText = document.createTextNode(QString::number(this->isVisible()));
    downloadEnableElement.appendChild(downloadEnableText);
}

bool DownloadAreaHighlight::restoreConfig(QDomDocument &document)
{
    bool result = false;
    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList downloadSettingsNodes = rootElem.elementsByTagName("DownloadAreaHighlight");

        for(int iMap = 0; iMap < downloadSettingsNodes.size(); iMap++)
        {
            QDomNode downloadSettingNode = downloadSettingsNodes.at(iMap);

            if(downloadSettingNode.isNull() == false)
            {
                QString value = AppSettings::getValueString(downloadSettingNode, "VisibleDownloadAreaHighlight");

                if(value.isEmpty() == false)
                {
                    int isVisible = value.toInt();
                    setVisible((isVisible > 0) ? true : false);
                    result = true;
                }
            }
        }
    }

    return result;
}
