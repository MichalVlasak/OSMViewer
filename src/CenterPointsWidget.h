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
        explicit CenterPointsWidget(CenterPointsManager & pointsManager, QWidget *parent = 0);
        ~CenterPointsWidget();

    private slots:
        void centerHome();
        void editHome();
        void centerToPointFromList(QModelIndex index);
        void centerToPointFromList();
        void addPointToCenterList();
        void editPointFromCenterList();
        void removePointFromCenterList();

    private:
        Ui::CenterPointsWidget * _ui;
        CenterPointsManager & _pointsManager;
        QStringListModel * _listModel = nullptr;
};

#endif // CENTERPOINTSWIDGET_H
