#ifndef OSMDOWNLOADAREADIALOG_H
#define OSMDOWNLOADAREADIALOG_H

#include <QDialog>

#include "DeleteOldMapsWidget.h"
#include "hmi/AreaGeometry.h"
#include "hmi/AreaGeometryWigdetInterface.h"

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

                AreaGeometry geometry;

                DeleteOldMapsWidget::DeleteSettings deleteSettings;
        };

    public:
        OSMDownloadAreaDialog(const Setup & setup, const QString & projectName, QWidget *parent = 0);
        ~OSMDownloadAreaDialog();

        Setup getCurrenSetup();

    private slots:
        void changeLevelFrom(int level);
        void changeLevelTo(int level);

        void setFromMinimum();
        void setToMaximum();

        void saveAsProject();

    private:
        Ui::OSMDownloadAreaDialog *_ui = nullptr;
        Setup _setup;
        QString _projectName;
        AreaGeometryWigdetInterface * _geometryWidget = nullptr;
};

#endif // OSMDOWNLOADAREADIALOG_H
