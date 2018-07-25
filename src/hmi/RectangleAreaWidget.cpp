#include "RectangleAreaWidget.h"
#include "ui_RectangleAreaWidget.h"

RectangleAreaWidget::RectangleAreaWidget(const AreaGeometry & geometry, QWidget *parent)
    : AreaGeometryWigdetInterface(parent),
      _ui(new Ui::RectangleAreaWidget)
{
    _ui->setupUi(this);

    if(geometry.geometryType == AreaGeometry::Type::Rectangle &&
       geometry.geometry.isNull() == false &&
       geometry.geometry.canConvert<QRectF>() == true)
    {

        QRectF rect = geometry.geometry.toRectF();

        _lonFrom = rect.topLeft().x();
        _latFrom = rect.topLeft().y();
        _lonTo = rect.bottomRight().x();
        _latTo = rect.bottomRight().y();

        if(_latFrom < -90.) _latFrom = -90.;
        if(_latFrom > 90.) _latFrom = 90.;

        if(_lonFrom < -180.) _lonFrom = -180.;
        if(_lonFrom > 180.) _lonFrom = 180.;

        if(_latTo < -90.) _latTo = -90.;
        if(_latTo > 90.) _latTo = 90.;

        if(_lonTo < -180.) _lonTo = -180.;
        if(_lonTo > 180.) _lonTo = 180.;

        if(_latFrom < _latTo) _latFrom = _latTo;
        if(_lonFrom > _lonTo) _lonFrom = _lonTo;

        _ui->latFrom->setValue(_latFrom);
        _ui->latFrom->setMinimum(_latTo);

        _ui->latTo->setValue(_latTo);
        _ui->latTo->setMaximum(_latFrom);

        _ui->lonFrom->setValue(_lonFrom);
        _ui->lonFrom->setMaximum(_lonTo);

        _ui->lonTo->setValue(_lonTo);
        _ui->lonTo->setMinimum(_lonFrom);
    }

    QObject::connect(_ui->latFrom, SIGNAL(valueChanged(double)), SLOT(changeLatFrom(double)));
    QObject::connect(_ui->latTo, SIGNAL(valueChanged(double)), SLOT(changeLatTo(double)));

    QObject::connect(_ui->lonFrom, SIGNAL(valueChanged(double)), SLOT(changeLonFrom(double)));
    QObject::connect(_ui->lonTo, SIGNAL(valueChanged(double)), SLOT(changeLonTo(double)));
}

RectangleAreaWidget::~RectangleAreaWidget()
{
    delete _ui;
}

void RectangleAreaWidget::changeLatFrom(double lat)
{
    _latFrom = lat;
    _ui->latTo->setMaximum(_latFrom);
}

void RectangleAreaWidget::changeLatTo(double lat)
{
    _latTo = lat;
    _ui->latFrom->setMinimum(_latTo);
}

void RectangleAreaWidget::changeLonFrom(double lon)
{
    _lonFrom = lon;
    _ui->lonTo->setMinimum(_lonFrom);
}

void RectangleAreaWidget::changeLonTo(double lon)
{
    _lonTo = lon;
    _ui->lonFrom->setMaximum(_lonTo);
}

QVariant RectangleAreaWidget::getGeometry()
{
    QVariant geom;

    geom = QRectF(QPointF(_lonFrom, _latFrom), QPointF(_lonTo, _latTo));;

    return geom;
}
