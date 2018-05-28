#ifndef GPXTABLEINFOFILEWIDGET_H
#define GPXTABLEINFOFILEWIDGET_H

#include <QWidget>

#include "GpxInfoFileModel.h"

namespace Ui {
class GpxInfoFileWidget;
}

class GpxInfoFileWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxInfoFileWidget(GpxManager * gpxManager, QWidget *parent = 0);
        ~GpxInfoFileWidget();

    public slots:
        void changeSelectedGps(const GpxManager::GpxIdVector & ids);

    private:
        Ui::GpxInfoFileWidget * _ui = nullptr;
        GpxInfoFileModel * _tableMode = nullptr;
        GpxManager * _gpxManager = nullptr;
};

#endif // GPXTABLEINFOFILEWIDGET_H
