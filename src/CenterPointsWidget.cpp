#include "CenterPointsWidget.h"
#include "ui_CenterPointsWidget.h"

#include "MainWindow.h"
#include "CenterPointEditDialog.h"

#include <QListView>

CenterPointsWidget::CenterPointsWidget(CenterPointsManager * pointsManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::CenterPointsWidget),
    _pointsManager(pointsManager)
{
    _ui->setupUi(this);

    _ui->homeName->setText(_pointsManager->getHomeCenterPoint().name);

    _listModel = new QStringListModel(this);
    _ui->listView->setModel(_listModel);

    refreshPointsList();

    QObject::connect(_ui->centerHomeButton, SIGNAL(clicked(bool)), SLOT(centerHome()));
    QObject::connect(_ui->editHomeButton, SIGNAL(clicked(bool)), SLOT(editHome()));
    QObject::connect(_ui->centerButton, SIGNAL(clicked(bool)), SLOT(centerToPointFromList()));
    QObject::connect(_ui->addButton, SIGNAL(clicked(bool)), SLOT(addPointToCenterList()));
    QObject::connect(_ui->editButton, SIGNAL(clicked(bool)), SLOT(editPointFromCenterList()));
    QObject::connect(_ui->deleteButton, SIGNAL(clicked(bool)), SLOT(removePointFromCenterList()));
    QObject::connect(_ui->listView, SIGNAL(doubleClicked(QModelIndex)), SLOT(centerToPointFromList(QModelIndex)));
    QObject::connect(_pointsManager, SIGNAL(homePointWasChanged()), SLOT(changeHome()));
    QObject::connect(_pointsManager, SIGNAL(pointsWasAdded()), SLOT(refreshPointsList()));
    QObject::connect(_pointsManager, SIGNAL(pointsWasRemoved()), SLOT(refreshPointsList()));
}

CenterPointsWidget::~CenterPointsWidget()
{
    delete _ui;
}

void CenterPointsWidget::refreshPointsList()
{
    _listModel->removeRows(0, _listModel->rowCount());

    const CenterPointsManager::CenterPointsVector & points = _pointsManager->getCenterPointsVector();

    QStringList pointsNameList;

    for(const CenterPointStruct & point : points)
    {
        pointsNameList << point.name;
    }

    _listModel->setStringList(pointsNameList);
}

void CenterPointsWidget::centerHome()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        mainWindow->centerToPoint(_pointsManager->getHomeCenterPoint());
    }
}

void CenterPointsWidget::editHome()
{
    CenterPointEditDialog * dialog = new CenterPointEditDialog(_pointsManager->getHomeCenterPoint(), this);

    if(dialog->exec() == QDialog::Accepted)
    {
        _pointsManager->setHomeCenterPoint(dialog->getCenterPoint());
        _ui->homeName->setText(_pointsManager->getHomeCenterPoint().name);
    }
}

void CenterPointsWidget::changeHome()
{
    _ui->homeName->setText(_pointsManager->getHomeCenterPoint().name);
}

void CenterPointsWidget::centerToPointFromList(QModelIndex index)
{
    if(_listModel != nullptr)
    {
        if(index.isValid() == true)
        {
            QString name = _listModel->data(index).toString();

            const CenterPointsManager::CenterPointsVector & points = _pointsManager->getCenterPointsVector();

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
    CenterPointEditDialog * dialog = new CenterPointEditDialog(CenterPointStruct(), this);

    if(dialog->exec() == QDialog::Accepted)
    {
        _pointsManager->addCenterPoint(dialog->getCenterPoint());
    }
}

void CenterPointsWidget::editPointFromCenterList()
{
    if(_listModel != nullptr)
    {
        QModelIndexList indexes = _ui->listView->selectionModel()->selectedIndexes();

        if(indexes.size() > 0)
        {
            const QModelIndex & index = indexes.at(0);

            if(index.isValid() == true)
            {
                QString name = _listModel->data(index).toString();

                const CenterPointsManager::CenterPointsVector & points = _pointsManager->getCenterPointsVector();

                for(const CenterPointStruct & point : points)
                {
                    if(name.compare(point.name) == 0)
                    {
                        CenterPointEditDialog * dialog = new CenterPointEditDialog(point, this);

                        if(dialog->exec() == QDialog::Accepted)
                        {
                            _pointsManager->removeCenterPoint(point);
                            _pointsManager->addCenterPoint(dialog->getCenterPoint());
                        }
                    }
                }
            }
        }
    }
}

void CenterPointsWidget::removePointFromCenterList()
{
    if(_listModel != nullptr)
    {
        QModelIndexList indexes = _ui->listView->selectionModel()->selectedIndexes();

        if(indexes.size() > 0)
        {
            const QModelIndex & index = indexes.at(0);

            if(index.isValid() == true)
            {
                QString name = _listModel->data(index).toString();

                const CenterPointsManager::CenterPointsVector & points = _pointsManager->getCenterPointsVector();

                for(const CenterPointStruct & point : points)
                {
                    if(name.compare(point.name) == 0)
                    {
                        _pointsManager->removeCenterPoint(point);
                    }
                }
            }
        }
    }
}
