#include "CircleWidget.h"
#include "ui_CircleWidget.h"
#include "../MainWindow.h"

#include <cmath>

CircleWidget::CircleWidget(const AreaGeometry & geometry, QWidget *parent)
    : AreaGeometryWigdetInterface(parent),
      _ui(new Ui::CircleWidget)
{
    _ui->setupUi(this);

    if(geometry.geometryType == AreaGeometry::Type::Circle &&
       geometry.geometry.isNull() == false &&
       geometry.geometry.canConvert<AreaGeometry::CircleGeometry>() == true)
    {
        AreaGeometry::CircleGeometry circleGeometry = geometry.geometry.value<AreaGeometry::CircleGeometry>();

        _ui->latitude->setValue(circleGeometry.centerPoint.y());
        _ui->longitude->setValue(circleGeometry.centerPoint.x());
        _ui->radius->setValue(circleGeometry.radius);
    }
}

CircleWidget::~CircleWidget()
{
    delete _ui;
}

AreaGeometry CircleWidget::getGeometry()
{
    AreaGeometry geom;
    AreaGeometry::CircleGeometry circle;

    circle.centerPoint = QPointF(_ui->longitude->value(), _ui->latitude->value());
    circle.radius = _ui->radius->value();
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        AreaGeometry::CircleGeometry::circleToPolygon(circle, window->getMapSettings());
    }

    geom.geometryType = AreaGeometry::Type::Circle;
    geom.geometry = QVariant::fromValue(circle);

    return geom;
}
