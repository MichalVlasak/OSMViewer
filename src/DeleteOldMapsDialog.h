#ifndef DELETEOLDMAPSDIALOG_H
#define DELETEOLDMAPSDIALOG_H

#include "DeleteOldMapsWidget.h"

#include <QDialog>

namespace Ui {
class DeleteOldMapsDialog;
}

class DeleteOldMapsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit DeleteOldMapsDialog(QWidget *parent = 0);
        ~DeleteOldMapsDialog();

        DeleteOldMapsWidget::DeleteSettings getDeleteSettings() const;
        void setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings);
        void setEnabledDeleteAll(bool enable);

    private:
        Ui::DeleteOldMapsDialog * _ui;
};

#endif // DELETEOLDMAPSDIALOG_H
