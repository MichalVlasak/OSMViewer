#ifndef GPXFILELISTWIDGET_H
#define GPXFILELISTWIDGET_H

#include <QWidget>

#include "GpxManager.h"
#include "GpxTableListModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxFileListWidget;
}

class GpxFileListWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxFileListWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent = 0);
        ~GpxFileListWidget();

    private slots:
        void addFile();
        void deleteFile();
        void deleteAllFile();
        void selectionChanged(QItemSelection selected, QItemSelection deselected);
        void clearSelection();

    private:
        void reloadGpx();
        int getId(const QModelIndex & index);

    private:
        Ui::GpxFileListWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxTableListModel * _tableModel = nullptr;
        GpxLayer * _gpxLayer = nullptr;
};

#endif // GPXFILELISTWIDGET_H
