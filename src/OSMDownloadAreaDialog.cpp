#include "OSMDownloadAreaDialog.h"
#include "ui_OSMDownloadAreaDialog.h"

#include "Layers/ZoomInfo.h"

#include <QSpinBox>

OSMDownloadAreaDialog::OSMDownloadAreaDialog(Setup & setup, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::OSMDownloadAreaDialog),
    _setup(setup)
{
    _ui->setupUi(this);

    if(_setup.levelFrom < 0) _setup.levelFrom = 0;
    if(_setup.levelFrom >= int(ZoomInfo::MaxZoomLevel)) _setup.levelFrom = ZoomInfo::MaxZoomLevel - 1;

    if(_setup.levelTo < 0) _setup.levelTo = 0;
    if(_setup.levelTo >= int(ZoomInfo::MaxZoomLevel)) _setup.levelTo = ZoomInfo::MaxZoomLevel - 1;

    if(_setup.latFrom < -90.) _setup.latFrom = -90.;
    if(_setup.latFrom > 90.) _setup.latFrom = 90.;

    if(_setup.lonFrom < -180.) _setup.lonFrom = -180.;
    if(_setup.lonFrom > 180.) _setup.lonFrom = 180.;

    if(_setup.latTo < -90.) _setup.latTo = -90.;
    if(_setup.latTo > 90.) _setup.latTo = 90.;

    if(_setup.lonTo < -180.) _setup.lonTo = -180.;
    if(_setup.lonTo > 180.) _setup.lonTo = 180.;

    if(_setup.latFrom < _setup.latTo) _setup.latFrom = _setup.latTo;
    if(_setup.lonFrom > _setup.lonTo) _setup.lonFrom = _setup.lonTo;

    _ui->levelFrom->setValue(_setup.levelFrom);
    _ui->levelFrom->setMaximum(_setup.levelTo);

    _ui->levelTo->setValue(_setup.levelTo);
    _ui->levelTo->setMinimum(_setup.levelFrom);

    _ui->latFrom->setValue(_setup.latFrom);
    _ui->latFrom->setMinimum(_setup.latTo);

    _ui->latTo->setValue(_setup.latTo);
    _ui->latTo->setMaximum(_setup.latFrom);

    _ui->lonFrom->setValue(_setup.lonFrom);
    _ui->lonFrom->setMaximum(_setup.lonTo);

    _ui->lonTo->setValue(_setup.lonTo);
    _ui->lonTo->setMinimum(_setup.lonFrom);

    _ui->deleteOldMapsWidget->setDeleteSettings(_setup.deleteSettings);

    QObject::connect(_ui->levelFrom, SIGNAL(valueChanged(int)), SLOT(changeLevelFrom(int)));
    QObject::connect(_ui->levelTo, SIGNAL(valueChanged(int)), SLOT(changeLevelTo(int)));

    QObject::connect(_ui->latFrom, SIGNAL(valueChanged(double)), SLOT(changeLatFrom(double)));
    QObject::connect(_ui->latTo, SIGNAL(valueChanged(double)), SLOT(changeLatTo(double)));

    QObject::connect(_ui->lonFrom, SIGNAL(valueChanged(double)), SLOT(changeLonFrom(double)));
    QObject::connect(_ui->lonTo, SIGNAL(valueChanged(double)), SLOT(changeLonTo(double)));
}

OSMDownloadAreaDialog::~OSMDownloadAreaDialog()
{
    delete _ui;
}

void OSMDownloadAreaDialog::changeLevelFrom(int level)
{
    _setup.levelFrom = level;
    _ui->levelTo->setMinimum(_setup.levelFrom);
}

void OSMDownloadAreaDialog::changeLevelTo(int level)
{
    _setup.levelTo = level;
    _ui->levelFrom->setMaximum(_setup.levelTo);
}

void OSMDownloadAreaDialog::changeLatFrom(double lat)
{
    _setup.latFrom = lat;
    _ui->latTo->setMaximum(_setup.latFrom);
}

void OSMDownloadAreaDialog::changeLatTo(double lat)
{
    _setup.latTo = lat;
    _ui->latFrom->setMinimum(_setup.latTo);
}

void OSMDownloadAreaDialog::changeLonFrom(double lon)
{
    _setup.lonFrom = lon;
    _ui->lonTo->setMinimum(_setup.lonFrom);
}

void OSMDownloadAreaDialog::changeLonTo(double lon)
{
    _setup.lonTo = lon;
    _ui->lonFrom->setMaximum(_setup.lonTo);
}

OSMDownloadAreaDialog::Setup OSMDownloadAreaDialog::getCurrenSetup()
{
    Setup setup;

    setup.levelFrom = _ui->levelFrom->value();
    setup.levelTo = _ui->levelTo->value();

    setup.latFrom = _ui->latFrom->value();
    setup.latTo = _ui->latTo->value();

    setup.lonFrom = _ui->lonFrom->value();
    setup.lonTo = _ui->lonTo->value();

    setup.deleteSettings = _ui->deleteOldMapsWidget->getDeleteSettings();

    return setup;
}
