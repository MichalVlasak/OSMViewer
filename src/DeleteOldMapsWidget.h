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

        /**
         * @brief setEnabledDeleteAll umozni zapnut a vypnut RadioButton pre
         * zvolenia vymazania vsetkych dlazdic bez kontroli datumu (pri stahovani
         * pocas prezerania je tato volba kontraproduktivna: vymaze stiahne zobrazi
         * vymaze stiahne zobrazi ... cize sa v podstate nic nezobrazi)
         * @param enable
         */
        void setEnabledDeleteAll(bool enable);

    private:
        Ui::DeleteOldMapsWidget *_ui = nullptr;
};

#endif // DELETEOLDMAPSWIDGET_H
