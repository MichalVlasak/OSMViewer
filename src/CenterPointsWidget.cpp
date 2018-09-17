#include "CenterPointsWidget.h"
#include "ui_CenterPointsWidget.h"

#include "MainWindow.h"
#include "CenterPointEditDialog.h"
#include "GroupEditDialog.h"

#include <QListView>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

CenterPointsWidget::CenterPointsWidget(CenterPointsManager * pointsManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::CenterPointsWidget),
    _pointsManager(pointsManager)
{
    _ui->setupUi(this);

    _ui->homeName->setText(_pointsManager->getHomeCenterPoint().name);

    _listModel = new QStringListModel(this);
    _ui->listView->setModel(_listModel);

    QObject::connect(_ui->centerHomeButton, SIGNAL(clicked(bool)), SLOT(centerHome()));
    QObject::connect(_ui->editHomeButton, SIGNAL(clicked(bool)), SLOT(editHome()));
    QObject::connect(_ui->centerButton, SIGNAL(clicked(bool)), SLOT(centerToPointFromList()));
    QObject::connect(_ui->addButton, SIGNAL(clicked(bool)), SLOT(addPointToCenterList()));
    QObject::connect(_ui->editButton, SIGNAL(clicked(bool)), SLOT(editPointFromCenterList()));
    QObject::connect(_ui->deleteButton, SIGNAL(clicked(bool)), SLOT(removePointFromCenterList()));
    QObject::connect(_ui->deleteAllButton, SIGNAL(clicked(bool)), SLOT(removeAllPointsFromCenterList()));
    QObject::connect(_ui->importButton, SIGNAL(clicked(bool)), SLOT(importPoints()));
    QObject::connect(_ui->exportAllButton, SIGNAL(clicked(bool)), SLOT(exportAllGroups()));
    QObject::connect(_ui->exportButton, SIGNAL(clicked(bool)), SLOT(exportCurrentGroup()));
    QObject::connect(_ui->listView, SIGNAL(clicked(QModelIndex)), SLOT(clickedToList(QModelIndex)));
    QObject::connect(_ui->listView, SIGNAL(doubleClicked(QModelIndex)), SLOT(centerToPointFromList(QModelIndex)));
    QObject::connect(_pointsManager, SIGNAL(homePointWasChanged()), SLOT(changeHome()));
    QObject::connect(_pointsManager, SIGNAL(pointsWasAdded()), SLOT(refreshCurrentGroup()));
    QObject::connect(_pointsManager, SIGNAL(pointsWasRemoved()), SLOT(refreshCurrentGroup()));
    QObject::connect(_ui->findLineEdit, SIGNAL(textChanged(QString)), SLOT(findTextChanged(QString)));
    QObject::connect(_ui->groupComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(changeGroup(QString)));
    QObject::connect(_ui->addGroup, SIGNAL(clicked(bool)), SLOT(addGroup()));
    QObject::connect(_ui->editGroup, SIGNAL(clicked(bool)), SLOT(editGroup()));
    QObject::connect(_ui->removeGroup, SIGNAL(clicked(bool)), SLOT(removeGroup()));

    _ui->centerButton->setEnabled(false);
    _ui->editButton->setEnabled(false);
    _ui->deleteButton->setEnabled(false);
}

CenterPointsWidget::~CenterPointsWidget()
{
    delete _ui;
}

void CenterPointsWidget::retranslate()
{
    _ui->retranslateUi(this);
}

void CenterPointsWidget::refreshCurrentGroup()
{
    changeGroup(_ui->groupComboBox->currentText());
}

void CenterPointsWidget::fillPointsList()
{
    _listModel->removeRows(0, _listModel->rowCount());

    const CenterPointsManager::CenterPointsMap & pointsMap = _pointsManager->getCenterPointsMap();
    CenterPointsManager::CenterPointsMap::const_iterator it, end = pointsMap.end();

    _ui->groupComboBox->clear();

    for(it = pointsMap.begin(); it != end; ++it)
    {
        const CenterPointsManager::CenterPointsVector & points = it->second;
        _ui->groupComboBox->addItem(it->first);
        _ui->groupComboBox->setCurrentText(it->first);
        _ui->groupComboBox->model()->sort(0);

        QStringList pointsNameList;

        int row = -1;
        int i = 0;

        for(const CenterPointStruct & point : points)
        {
            if(_findText.isEmpty() == true)
            {
                pointsNameList << point.name;

                if(_lastAdded.isEmpty() == false)
                {
                    if(point.name.compare(_lastAdded) == 0)
                    {
                        row = i;
                    }

                    i++;
                }
            }
            else
            {
                if(point.name.indexOf(_findText, 0, Qt::CaseInsensitive) >= 0)
                {
                    pointsNameList << point.name;
                }
            }
        }

        _listModel->setStringList(pointsNameList);

        if(row != -1)
        {
            _ui->listView->selectionModel()->setCurrentIndex(_listModel->index(row), QItemSelectionModel::Select);
        }
    }
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
            QString pointName = _listModel->data(index).toString();
            QString groupName = _ui->groupComboBox->currentText();

            const CenterPointsManager::CenterPointsVector * points = _pointsManager->getCenterPointsVector(groupName);

            if(points != nullptr)
            {
                for(const CenterPointStruct & point : *points)
                {
                    if(pointName.compare(point.name) == 0)
                    {
                        MainWindow * mainWindow = MainWindow::getInstance();

                        if(mainWindow != nullptr)
                        {
                            mainWindow->centerToPoint(point);
                            mainWindow->setFocus();
                        }
                    }
                }
            }
        }
    }
}

void CenterPointsWidget::clickedToList(QModelIndex index)
{
    if(index.isValid() == true)
    {
        _ui->centerButton->setEnabled(true);
        _ui->editButton->setEnabled(true);
        _ui->deleteButton->setEnabled(true);

        MainWindow * mainWindow = MainWindow::getInstance();

        if(mainWindow != nullptr)
        {
            mainWindow->setFocus();
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
        CenterPointStruct point = dialog->getCenterPoint();
        _lastAdded = point.name;
        QString groupName = _ui->groupComboBox->currentText();

        _pointsManager->addCenterPoint(groupName, point, true);
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
                QString pointName = _listModel->data(index).toString();
                QString groupName = _ui->groupComboBox->currentText();

                const CenterPointsManager::CenterPointsVector * points = _pointsManager->getCenterPointsVector(groupName);

                if(points != nullptr)
                {
                    for(const CenterPointStruct & point : *points)
                    {
                        if(pointName.compare(point.name) == 0)
                        {
                            CenterPointEditDialog * dialog = new CenterPointEditDialog(point, this);

                            if(dialog->exec() == QDialog::Accepted)
                            {
                                _pointsManager->removeCenterPoint(groupName, point, true);
                                _pointsManager->addCenterPoint(groupName, dialog->getCenterPoint(), true);
                            }

                            delete dialog;
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
                QString pointName = _listModel->data(index).toString();
                QString groupName = _ui->groupComboBox->currentText();

                const CenterPointsManager::CenterPointsVector * points = _pointsManager->getCenterPointsVector(groupName);

                if(points != nullptr)
                {
                    for(const CenterPointStruct & point : *points)
                    {
                        if(pointName.compare(point.name) == 0)
                        {
                            QMessageBox::StandardButton reply;

                            reply = QMessageBox::question(this, tr("Delete Point"), tr("Are You sure to delete point \"") + point.name + tr("\"?"));

                            if(reply == QMessageBox::Yes)
                            {
                                _pointsManager->removeCenterPoint(groupName, point, true);

                                // po vymazani bodu musim ukoncit cyklus, pretoze sa mi
                                // zmenil vektor na ktory mam referenciu a teda zostanem
                                // v nekonzistentnom stave.
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void CenterPointsWidget::removeAllPointsFromCenterList()
{
    if(_pointsManager != nullptr)
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, tr("Delete All Points"), tr("Are You sure to delete all points?"));

        if(reply == QMessageBox::Yes)
        {
            _pointsManager->removeAllCenterPoints();
        }
    }

    fillPointsList();
}

QString CenterPointsWidget::getCurrentGroupName() const
{
    return _ui->groupComboBox->currentText();
}

void CenterPointsWidget::importPoints()
{
    if(_pointsManager != nullptr)
    {
        _pointsManager->importPoints();
    }

    fillPointsList();
}

void CenterPointsWidget::exportAllGroups()
{
    if(_pointsManager != nullptr)
    {
        _pointsManager->exportAllGroups();
    }
}

void CenterPointsWidget::exportCurrentGroup()
{
    if(_pointsManager != nullptr)
    {
        _pointsManager->exportGroup(_ui->groupComboBox->currentText());
    }
}

void CenterPointsWidget::findTextChanged(const QString &findText)
{
    _findText = findText;
    _lastAdded.clear();

    QStringList pointsNameList = getPointsListForGroup(_ui->groupComboBox->currentText());
    QStringList newPointsNameList;

    for(const QString & pointName : pointsNameList)
    {
        if(pointName.indexOf(_findText, 0, Qt::CaseInsensitive) >= 0)
        {
            newPointsNameList << pointName;
        }
    }

    _listModel->setStringList(newPointsNameList);
}

void CenterPointsWidget::setLastAdded(const QString &pointName)
{
    _lastAdded = pointName;
}

void CenterPointsWidget::changeGroup(const QString &groupName)
{
    _listModel->removeRows(0, _listModel->rowCount());
    QStringList pointsNameList = getPointsListForGroup(groupName);
    QStringList newPointsNameList;
    int row = -1;
    int i = 0;

    for(const QString & pointName : pointsNameList)
    {
        if(_findText.isEmpty() == true)
        {
            newPointsNameList << pointName;

            if(_lastAdded.isEmpty() == false)
            {
                if(pointName.compare(_lastAdded) == 0)
                {
                    row = i;
                }

                i++;
            }
        }
        else
        {
            if(pointName.indexOf(_findText, 0, Qt::CaseInsensitive) >= 0)
            {
                newPointsNameList << pointName;
            }
        }
    }

    _listModel->setStringList(newPointsNameList);

    if(row != -1)
    {
        _ui->listView->selectionModel()->setCurrentIndex(_listModel->index(row), QItemSelectionModel::Select);
        _ui->centerButton->setEnabled(true);
        _ui->editButton->setEnabled(true);
        _ui->deleteButton->setEnabled(true);
    }
}

QStringList CenterPointsWidget::getPointsListForGroup(const QString &groupName)
{
    const CenterPointsManager::CenterPointsMap & pointsMap = _pointsManager->getCenterPointsMap();
    CenterPointsManager::CenterPointsMap::const_iterator it = pointsMap.find(groupName);

    QStringList pointsNameList;

    if(it != pointsMap.end())
    {
        const CenterPointsManager::CenterPointsVector & points = it->second;

        for(const CenterPointStruct & point : points)
        {
            pointsNameList << point.name;
        }

        _listModel->setStringList(pointsNameList);
    }

    return pointsNameList;
}

void CenterPointsWidget::addGroup()
{
    GroupEditDialog * dialog = new GroupEditDialog("", this);

    if(dialog->exec() == QDialog::Accepted)
    {
        QString groupName = dialog->getGroupName();

        if(_pointsManager->createNewGroup(groupName) == true)
        {
            _ui->groupComboBox->addItem(groupName);
            _ui->groupComboBox->setCurrentText(groupName);
            _ui->groupComboBox->model()->sort(0);
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"), tr("Cannot create new group!\nGroup \"") + groupName + tr("\" is exists!"));
        }
    }
}

void CenterPointsWidget::editGroup()
{
    QString oldGroupName = _ui->groupComboBox->currentText();
    GroupEditDialog * dialog = new GroupEditDialog(oldGroupName, this);

    if(dialog->exec() == QDialog::Accepted)
    {
        QString groupName = dialog->getGroupName();

        if(_pointsManager->changeGroupName(oldGroupName, groupName) == true)
        {
            fillPointsList();
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"), tr("Cannot change \"") + oldGroupName + tr("\" to group \"") + groupName + tr("\" group!\nGroup \"") + groupName + tr("\" is exists!"));
        }
    }
}

void CenterPointsWidget::removeGroup()
{
    QString groupName = _ui->groupComboBox->currentText();

    QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Remove group"), tr("Are you sure to remove group \"") + groupName + tr("\"?"));

    if(btn == QMessageBox::Ok || btn == QMessageBox::Yes)
    {
        if(_pointsManager->removeGroup(groupName) == true)
        {
            fillPointsList();
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"), tr("Cannot remove group \"") + groupName + tr("\"!"));
        }
    }
}
