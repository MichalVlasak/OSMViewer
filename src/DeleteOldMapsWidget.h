#ifndef DELETEOLDMAPSWIDGET_H
#define DELETEOLDMAPSWIDGET_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class DeleteOldMapsWidget;
}

class DeleteOldMapsWidget : public QWidget
{
        Q_OBJECT

    public:
        enum DeleteType
        {
            DeleteOldAsTime,
            DeleteAll
        };

        struct DeleteSettings
        {
                bool deleteEnabled = false;
                DeleteType deleteType = DeleteAll;
                QDateTime deleteTime;
        };

    public:
        explicit DeleteOldMapsWidget(QWidget *parent = 0);
        ~DeleteOldMapsWidget();

        DeleteSettings getDeleteSettings() const;
        void setDeleteSettings(DeleteSettings settings);
        void setEnabledDeleteAll(bool enable);

    private:
        Ui::DeleteOldMapsWidget *_ui;
};

#endif // DELETEOLDMAPSWIDGET_H
