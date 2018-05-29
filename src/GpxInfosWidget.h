#ifndef GPXINFOSWIDGET_H
#define GPXINFOSWIDGET_H

#include <QWidget>

#include "GpxInfoFileModel.h"

namespace Ui {
class GpxInfosWidget;
}

class GpxInfosWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxInfosWidget(GpxManager * gpxManager, QWidget *parent = 0);
        ~GpxInfosWidget();

    public slots:
        void changeSelectedGps(const GpxManager::GpxIdVector & ids);
        void deleteGpx(int id);
        void deleteAll();

    private:
        Ui::GpxInfosWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxManager::GpxIdVector _showedGpxIds;
};

#endif // GPXINFOSWIDGET_H
