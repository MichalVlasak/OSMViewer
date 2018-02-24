#ifndef OSMDOWNLOADAREADIALOG_H
#define OSMDOWNLOADAREADIALOG_H

#include <QDialog>

#include "DeleteOldMapsWidget.h"

namespace Ui {
class OSMDownloadAreaDialog;
}

class OSMDownloadAreaDialog : public QDialog
{
        Q_OBJECT

    public:
        struct Setup
        {
                int levelFrom = 0;
                int levelTo = 0;

                double latFrom = 0.;
                double latTo = 0.;
                double lonFrom = 0.;
                double lonTo = 0.;

                DeleteOldMapsWidget::DeleteSettings deleteSettings;
        };

    public:
        explicit OSMDownloadAreaDialog(Setup & setup, const QString & projectName, QWidget *parent = 0);
        ~OSMDownloadAreaDialog();

        Setup getCurrenSetup();

    private slots:
        void changeLevelFrom(int level);
        void changeLevelTo(int level);

        void changeLatFrom(double lat);
        void changeLatTo(double lat);

        void changeLonFrom(double lon);
        void changeLonTo(double lon);

        void setFromMinimum();
        void setToMaximum();

        void saveAsProject();

    private:
        Ui::OSMDownloadAreaDialog *_ui;
        Setup _setup;
        QString _projectName;
};

#endif // OSMDOWNLOADAREADIALOG_H
