#include "OSMDownloadProjectWidget.h"
#include "ui_OSMDownloadProjectWidget.h"
#include "OSMDownloadProjectModel.h"
#include "MainWindow.h"

#include <QListView>
#include <QPushButton>

OSMDownloadProjectWidget::OSMDownloadProjectWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::OSMDownloadProjectWidgetForm)
{
    _ui->setupUi(this);

    _listModel = new QStringListModel(this);
    _ui->projectsListView->setModel(_listModel);

    QObject::connect(_ui->projectsListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(openProject(QModelIndex)));
    QObject::connect(_ui->loadProject, SIGNAL(clicked(bool)), SLOT(openProject()));
    QObject::connect(_ui->deleteProject, SIGNAL(clicked(bool)), SLOT(deleteProject()));
    QObject::connect(_ui->createProject, SIGNAL(clicked(bool)), SLOT(createProject()));
    QObject::connect(_ui->updateProject, SIGNAL(clicked(bool)), SLOT(updateProject()));
}

OSMDownloadProjectWidget::~OSMDownloadProjectWidget()
{
    delete _ui;
}

void OSMDownloadProjectWidget::initialize()
{
    reloadProjects();

    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadProjectModel & model = mainWindow->getOSMDownloadProjectModel();
        QObject::connect(&model, SIGNAL(modelChanged()), SLOT(reloadProjects()));
    }
}

void OSMDownloadProjectWidget::reloadProjects()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadProjectModel & model = mainWindow->getOSMDownloadProjectModel();
        const OSMDownloadProjectModel::ProjectVector & projects = model.getProjects();
        QStringList projectsName;

        for(const OSMDownloadProjectModel::Project & project : projects)
        {
            projectsName << project.name;
        }

        projectsName.sort();

        _listModel->setStringList(projectsName);
    }
}

void OSMDownloadProjectWidget::openProject(QModelIndex index)
{
    openProject(index.data().toString());
}

void OSMDownloadProjectWidget::openProject()
{
    QModelIndex index = _ui->projectsListView->selectionModel()->currentIndex();

    openProject(index.data().toString());
}

void OSMDownloadProjectWidget::openProject(QString projectName)
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadProjectModel & model = mainWindow->getOSMDownloadProjectModel();
        const OSMDownloadProjectModel::ProjectVector & projects = model.getProjects();

        for(const OSMDownloadProjectModel::Project & project : projects)
        {
            if(project.name.compare(projectName) == 0)
            {
                OSMDownloadAreaDialog::Setup setup = project.setup;

                mainWindow->showDownloadAreaDialog(setup, project.name);
            }
        }
    }
}

void OSMDownloadProjectWidget::deleteProject()
{
    QModelIndex index = _ui->projectsListView->selectionModel()->currentIndex();

    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadProjectModel & model = mainWindow->getOSMDownloadProjectModel();

        model.deleteProject(index.data().toString());

        reloadProjects();
    }
}

void OSMDownloadProjectWidget::createProject()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadAreaDialog::Setup setup;

        mainWindow->showDownloadAreaDialog(setup);
    }
}

void OSMDownloadProjectWidget::updateProject()
{
    openProject();
}
