#include "OSMDownloadAreaDialog.h"
#include "ui_OSMDownloadAreaDialog.h"
#include "MainWindow.h"
#include "OSMDownloadProjectModel.h"
#include "ProjectNameDialog.h"
#include "Layers/ZoomInfo.h"
#include "hmi/RectangleAreaWidget.h"

#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <exception>

OSMDownloadAreaDialog::OSMDownloadAreaDialog(const Setup & setup, const QString & projectName, QWidget *parent)
    : QDialog(parent),
      _ui(new Ui::OSMDownloadAreaDialog),
      _setup(setup),
      _projectName(projectName)
{
    _ui->setupUi(this);

    if(_setup.levelFrom < 0) _setup.levelFrom = 0;
    if(_setup.levelFrom >= int(ZoomInfo::MaxZoomLevel)) _setup.levelFrom = ZoomInfo::MaxZoomLevel - 1;

    if(_setup.levelTo < 0) _setup.levelTo = 0;
    if(_setup.levelTo >= int(ZoomInfo::MaxZoomLevel)) _setup.levelTo = ZoomInfo::MaxZoomLevel - 1;

    _ui->levelFrom->setValue(_setup.levelFrom);
    _ui->levelFrom->setMaximum(_setup.levelTo);

    _ui->levelTo->setValue(_setup.levelTo);
    _ui->levelTo->setMinimum(_setup.levelFrom);

    _geometryWidget = AreaGeometryWigdetInterface::createGeometryWidget(setup.geometry, this);
    QHBoxLayout * hLayout = new QHBoxLayout(this);

    hLayout->addWidget(_geometryWidget);

    _ui->geometryWidget->setLayout(hLayout);

    _ui->deleteOldMapsWidget->setDeleteSettings(_setup.deleteSettings);

    QObject::connect(_ui->levelFrom, SIGNAL(valueChanged(int)), SLOT(changeLevelFrom(int)));
    QObject::connect(_ui->levelTo, SIGNAL(valueChanged(int)), SLOT(changeLevelTo(int)));

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

OSMDownloadAreaDialog::Setup OSMDownloadAreaDialog::getCurrenSetup()
{
    Setup setup;

    setup.levelFrom = _ui->levelFrom->value();
    setup.levelTo = _ui->levelTo->value();

    setup.geometry.geometryType = _setup.geometry.geometryType;

    if(_geometryWidget != nullptr)
    {
        setup.geometry.geometry = _geometryWidget->getGeometry();
    }

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
