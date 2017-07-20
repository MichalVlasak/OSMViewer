#include "BaseLayer.h"

#include <QWidget>
#include <iostream>

BaseLayer::BaseLayer(MapSettings & mapSettings)
    : _mapSettings(mapSettings)
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
