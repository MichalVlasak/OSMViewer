#ifndef GPXWIDGET_H
#define GPXWIDGET_H

#include <QWidget>

#include "GpxManager.h"

namespace Ui {
class GpxWidget;
}

class GpxWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit GpxWidget(GpxManager * gpxManager, QWidget *parent = 0);
        ~GpxWidget();

    private slots:
        void addFile();

    private:
        Ui::GpxWidget * _ui;
        GpxManager * _gpxManager = nullptr;
};

#endif // GPXWIDGET_H
