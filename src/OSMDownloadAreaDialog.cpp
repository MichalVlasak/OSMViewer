#include "OSMDownloadAreaDialog.h"
#include "ui_OSMDownloadAreaDialog.h"
#include "MainWindow.h"
#include "OSMDownloadProjectModel.h"
#include "ProjectNameDialog.h"
#include "Layers/ZoomInfo.h"

#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

OSMDownloadAreaDialog::OSMDownloadAreaDialog(Setup & setup, const QString & projectName, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::OSMDownloadAreaDialog),
    _setup(setup),
    _projectName(projectName)
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

    QObject::connect(_ui->setFromMinimum, SIGNAL(clicked(bool)), SLOT(setFromMinimum()));
    QObject::connect(_ui->setToMaximum, SIGNAL(clicked(bool)), SLOT(setToMaximum()));

    QObject::connect(_ui->saveAsProject, SIGNAL(clicked(bool)), SLOT(saveAsProject()));
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

void OSMDownloadAreaDialog::setFromMinimum()
{
    _ui->levelFrom->setValue(_ui->levelFrom->minimum());
}

void OSMDownloadAreaDialog::setToMaximum()
{
    _ui->levelTo->setValue(_ui->levelTo->maximum());
}

void OSMDownloadAreaDialog::saveAsProject()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        ProjectNameDialog * dialog = new ProjectNameDialog(_projectName, this);

        if(dialog->exec() == QDialog::Accepted)
        {
            OSMDownloadProjectModel & projectModel = mainWindow->getOSMDownloadProjectModel();
            OSMDownloadProjectModel::Project project;

            project.setup = getCurrenSetup();
            project.name = dialog->getProjectName();

            if(projectModel.addProject(project) == false) // projekt sa uz nachadza v zozname
            {
                QMessageBox::StandardButton reply;

                reply = QMessageBox::question(this, tr("Overwrite Project"), tr("Are You sure to overwrite project \"") + project.name + tr("\"?"));

                if(reply == QMessageBox::Yes)
                {
                    projectModel.updateProject(project);
                }
            }
        }

        delete dialog;
    }
}
