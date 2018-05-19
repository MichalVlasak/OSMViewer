#ifndef GPXWIDGET_H
#define GPXWIDGET_H

#include <QWidget>

#include "GpxManager.h"
#include "GpxTableModel.h"

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
        void deleteFile();
        void deleteAllFile();
        void selectionChanged(QItemSelection selected, QItemSelection deselected);

    private:
        void reloadGpx();

    private:
        Ui::GpxWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxTableModel * _tableModel = nullptr;
};

#endif // GPXWIDGET_H
