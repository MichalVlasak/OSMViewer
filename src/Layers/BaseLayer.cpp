#include "BaseLayer.h"

#include <QWidget>
#include <iostream>

BaseLayer::BaseLayer(const QString & layerName, MapSettings & mapSettings)
    : _mapSettings(mapSettings),
      _layerName(layerName),
      _isVisible(true)
{
}

void BaseLayer::mouseMoveEvent(QMouseEvent *mouseEvent)
{
}

void BaseLayer::mousePressEvent(QMouseEvent *mouseEvent)
{
}

void BaseLayer::wheelEvent(QWheelEvent *wheelEvent)
{
}

void BaseLayer::keyPressEvent(QKeyEvent *keyEvent)
{
}

void BaseLayer::setVisible(bool visible)
{
    _isVisible = visible;
}

bool BaseLayer::isVisible() const
{
    return _isVisible;
}

const QString & BaseLayer::getLayerName() const
{
    return _layerName;
}
