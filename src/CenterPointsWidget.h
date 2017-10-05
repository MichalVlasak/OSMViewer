#ifndef CENTERPOINTSWIDGET_H
#define CENTERPOINTSWIDGET_H

#include <QWidget>

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

    private:
        Ui::CenterPointsWidget * _ui;
        CenterPointsManager & _pointsManager;
};

#endif // CENTERPOINTSWIDGET_H
