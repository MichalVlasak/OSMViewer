#include "CenterPointsWidget.h"
#include "ui_CenterPointsWidget.h"

#include "MainWindow.h"
#include "CenterPointEditDialog.h"

#include <QListView>

CenterPointsWidget::CenterPointsWidget(CenterPointsManager & pointsManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::CenterPointsWidget),
    _pointsManager(pointsManager)
{
    _ui->setupUi(this);

    _ui->homeName->setText(_pointsManager.getHomeCenterPoint().name);

    _listModel = new QStringListModel(this);
    _ui->listView->setModel(_listModel);

    const CenterPointsManager::CenterPointsVector & points = _pointsManager.getCenterPointsVector();

    QStringList pointsNameList;

    for(const CenterPointStruct & point : points)
    {
        pointsNameList << point.name;
    }

    _listModel->setStringList(pointsNameList);

    QObject::connect(_ui->centerHomeButton, SIGNAL(clicked(bool)), SLOT(centerHome()));
    QObject::connect(_ui->editHomeButton, SIGNAL(clicked(bool)), SLOT(editHome()));
    QObject::connect(_ui->centerButton, SIGNAL(clicked(bool)), SLOT(centerToPointFromList()));
    QObject::connect(_ui->listView, SIGNAL(doubleClicked(QModelIndex)), SLOT(centerToPointFromList(QModelIndex)));
}

CenterPointsWidget::~CenterPointsWidget()
{
    delete _ui;
}

void CenterPointsWidget::centerHome()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        mainWindow->centerToPoint(_pointsManager.getHomeCenterPoint());
    }
}

void CenterPointsWidget::editHome()
{
    CenterPointEditDialog * dialog = new CenterPointEditDialog(_pointsManager.getHomeCenterPoint(), this);

    if(dialog->exec() == QDialog::Accepted)
    {
    }
}

void CenterPointsWidget::centerToPointFromList(QModelIndex index)
{
    if(_listModel != nullptr)
    {
        if(index.isValid() == true)
        {
            QString name = _listModel->data(index).toString();

            const CenterPointsManager::CenterPointsVector & points = _pointsManager.getCenterPointsVector();

            for(const CenterPointStruct & point : points)
            {
                if(name.compare(point.name) == 0)
                {
                    MainWindow * mainWindow = MainWindow::getInstance();

                    if(mainWindow != nullptr)
                    {
                        mainWindow->centerToPoint(point);
                    }
                }
            }
        }
    }
}

void CenterPointsWidget::centerToPointFromList()
{
    QModelIndex index = _ui->listView->selectionModel()->currentIndex();

    centerToPointFromList(index);
}

void CenterPointsWidget::addPointToCenterList()
{
}

void CenterPointsWidget::editPointFromCenterList()
{
}

void CenterPointsWidget::removePointFromCenterList()
{
}
