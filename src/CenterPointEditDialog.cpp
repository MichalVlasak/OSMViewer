#include "CenterPointEditDialog.h"
#include "ui_CenterPointEditDialog.h"

#include "WgsConversion.h"

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>

CenterPointEditDialog::CenterPointEditDialog(const CenterPointStruct & point, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::CenterPointEditDialog)
{
    _ui->setupUi(this);

    _ui->name->setText(point.name);
    _ui->latitude->setValue(point.position.y());
    _ui->longitude->setValue(point.position.x());

    if(point.level != CenterPointStruct::UndefinedLevel)
    {
        _ui->levelCheckBox->setChecked(true);
        _ui->level->setValue(point.level);
    }
    else
    {
        _ui->levelCheckBox->setChecked(false);
    }

    refreshWgsLine();

    QObject::connect(_ui->latitude, SIGNAL(valueChanged(double)), SLOT(refreshWgsLine()));
    QObject::connect(_ui->longitude, SIGNAL(valueChanged(double)), SLOT(refreshWgsLine()));
    QObject::connect(_ui->wgsLineEdit, SIGNAL(textEdited(QString)), SLOT(refreshLatLonSpinBox()));
}

CenterPointEditDialog::~CenterPointEditDialog()
{
    delete _ui;
}

CenterPointStruct CenterPointEditDialog::getCenterPoint()
{
    CenterPointStruct point;

    point.name = _ui->name->text();
    point.level = (_ui->levelCheckBox->isChecked() == true) ? _ui->level->value() : CenterPointStruct::UndefinedLevel;
    point.position = QPointF(_ui->longitude->value(), _ui->latitude->value());

    return point;
}

void CenterPointEditDialog::refreshWgsLine()
{
    QString latString = WgsConversion::convertDoubleDegToWgs(_ui->latitude->value(), WgsConversion::Latitude, true, true);
    QString lonString = WgsConversion::convertDoubleDegToWgs(_ui->longitude->value() , WgsConversion::Longitude, true, true);

    QObject::disconnect(_ui->wgsLineEdit, SIGNAL(textEdited(QString)), this, SLOT(refreshLatLonSpinBox()));

    _ui->wgsLineEdit->setText(lonString + " " + latString);

    QObject::connect(_ui->wgsLineEdit, SIGNAL(textEdited(QString)), SLOT(refreshLatLonSpinBox()));
}

void CenterPointEditDialog::refreshLatLonSpinBox()
{
    QString wgs = _ui->wgsLineEdit->text();
    double lat = 0.;
    double lon = 0.;

    if(WgsConversion::convertWgsToDouble(wgs, lon, lat) == true)
    {
        QObject::disconnect(_ui->latitude, SIGNAL(valueChanged(double)), this, SLOT(refreshWgsLine()));
        QObject::disconnect(_ui->longitude, SIGNAL(valueChanged(double)), this, SLOT(refreshWgsLine()));

        _ui->longitude->setValue(lon);
        _ui->latitude->setValue(lat);

        QObject::connect(_ui->latitude, SIGNAL(valueChanged(double)), SLOT(refreshWgsLine()));
        QObject::connect(_ui->longitude, SIGNAL(valueChanged(double)), SLOT(refreshWgsLine()));
    }
}
