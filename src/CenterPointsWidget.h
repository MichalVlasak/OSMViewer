#ifndef CENTERPOINTSWIDGET_H
#define CENTERPOINTSWIDGET_H

#include <QWidget>
#include <QStringListModel>

#include "CenterPointsManager.h"

namespace Ui {
class CenterPointsWidget;
}

class CenterPointsWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit CenterPointsWidget(CenterPointsManager * pointsManager, QWidget *parent = 0);
        ~CenterPointsWidget();

    public:
        void setLastAdded(const QString & pointName);
        QString getCurrentGroupName() const;
        void fillPointsList();

    private slots:
        void centerHome();
        void editHome();
        void changeHome();
        void clickedToList(QModelIndex index);
        void centerToPointFromList(QModelIndex index);
        void centerToPointFromList();
        void addPointToCenterList();
        void editPointFromCenterList();
        void removePointFromCenterList();
        void removeAllPointsFromCenterList();
        void importPoints();
        void exportCurrentGroup();
        void exportAllGroups();
        void findTextChanged(const QString & findText);
        void changeGroup(const QString & groupName);
        void refreshCurrentGroup();
        void addGroup();
        void editGroup();
        void removeGroup();

    private:
        QStringList getPointsListForGroup(const QString & groupName);

    private:
        Ui::CenterPointsWidget * _ui;
        CenterPointsManager * _pointsManager;
        QStringListModel * _listModel = nullptr;
        QString _lastAdded;
        QString _findText;
};

#endif // CENTERPOINTSWIDGET_H
