#ifndef GPXWIDGET_H
#define GPXWIDGET_H

#include <QWidget>

#include "GpxManager.h"
#include "GpxTableModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxWidget;
}

class GpxWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent = 0);
        ~GpxWidget();

    private slots:
        void addFile();
        void deleteFile();
        void deleteAllFile();
        void selectionChanged(QItemSelection selected, QItemSelection deselected);

    private:
        void reloadGpx();
        int getId(const QModelIndex & index);

    private:
        Ui::GpxWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxTableModel * _tableModel = nullptr;
        GpxLayer * _gpxLayer = nullptr;
};

#endif // GPXWIDGET_H
