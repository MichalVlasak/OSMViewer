#ifndef GPXFILESLISTWIDGET_H
#define GPXFILESLISTWIDGET_H

#include <QWidget>

#include "GpxManager.h"
#include "GpxFilesListModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxFileListWidget;
}

class GpxFilesListWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxFilesListWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent = 0);
        ~GpxFilesListWidget();

    signals:
        void changeSelectedGpsSignal(const GpxManager::GpxIdVector & ids);
        void deleteGpxSignal(int id);
        void deleteAllSignal();

    private slots:
        void addFile();
        void deleteFile();
        void deleteAllFile();
        void downloadTilesForGpx();
        void selectionChanged(QItemSelection selected, QItemSelection deselected);
        void clearSelection();
        void gpxWasLoadedSlot(int fileId);
        void gpxStatusLoad(int allCount, int currentCounter);
        void gpxStatusAllLoaded();
        void gpxCurrentLoadingSignals(QString filePath);
        void checkDownloadRunning();

    private:
        void reloadGpx();
        int getId(const QModelIndex & index);

    private:
        Ui::GpxFileListWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxFilesListModel * _tableModel = nullptr;
        GpxLayer * _gpxLayer = nullptr;
        QTimer * _timer = nullptr;
};

#endif // GPXFILESLISTWIDGET_H
