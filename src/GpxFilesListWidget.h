#ifndef GPXFILESLISTWIDGET_H
#define GPXFILESLISTWIDGET_H

#include <QWidget>

#include "GpxManager.h"
#include "GpxFilesListModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxFilesListWidget;
}

class GpxFilesListWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxFilesListWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent = 0);
        ~GpxFilesListWidget();

        void setEnabledClearHighlightedButton(bool enabled);
        void retranslate();

    signals:
        void changeSelectedGpsSignal(const GpxManager::GpxIdVector & ids);
        void deleteGpxSignal(int id);
        void deleteAllSignal();
        void dataChanged(QModelIndex topLeft, QModelIndex bottomRight);
        void cancelLoadSignal();

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
        void clearHighlighted();

    private:
        void reloadGpx();
        int getId(const QModelIndex & index);

    private:
        Ui::GpxFilesListWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxFilesListModel * _tableModel = nullptr;
        GpxLayer * _gpxLayer = nullptr;
        QTimer * _timer = nullptr;
};

#endif // GPXFILESLISTWIDGET_H
