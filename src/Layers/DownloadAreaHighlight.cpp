#include "DownloadAreaHighlight.h"

#include <QPainter>

DownloadAreaHighlight::DownloadAreaHighlight(MapSettings & mapSettings)
    : BaseLayer(mapSettings)
{
    clearDownloadParams();
}

void DownloadAreaHighlight::paintEvent(QPaintEvent *paintEvent)
{
    if(_isDownloadParamsSetupEnable == true && _isVisible == true)
    {
        QPainter painter(_mapSettings.widget);

        QPoint startPointSelectArea(_mapSettings.getPixelForLon(_setup.lonFrom), _mapSettings.getPixelForLat(_setup.latFrom));
        QPoint endPointSelectArea(_mapSettings.getPixelForLon(_setup.lonTo), _mapSettings.getPixelForLat(_setup.latTo));

        QRect rect = QRect(startPointSelectArea, endPointSelectArea);

        painter.drawRect(rect);
        painter.fillRect(rect, QBrush(QColor(255, 0, 0, 50)));
    }
}

void DownloadAreaHighlight::setDownloadParams(OSMDownloadAreaDialog::Setup setup)
{
    _setup = setup;
    _isDownloadParamsSetupEnable = true;
}

void DownloadAreaHighlight::resetDownloadParams()
{
    clearDownloadParams();

    _mapSettings.widget->repaint();
}

void DownloadAreaHighlight::clearDownloadParams()
{
    _setup.latFrom = 0.;
    _setup.latTo = 0.;
    _setup.lonFrom = 0.;
    _setup.lonTo = 0.;
    _setup.levelFrom = 0.;
    _setup.levelTo = 0.;
    _isDownloadParamsSetupEnable = false;
}

void DownloadAreaHighlight::setVisible(bool value)
{
    _isVisible = value;

    _mapSettings.widget->repaint();
}

bool DownloadAreaHighlight::isVisible()
{
    return _isVisible;
}
