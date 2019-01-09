#ifndef OSMDIRECTORYPATHDIALOG_H
#define OSMDIRECTORYPATHDIALOG_H

#include <QDialog>

namespace Ui {
class OSMDirectoryPathDialog;
}

class OSMDirectoryPathDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit OSMDirectoryPathDialog(QWidget *parent = 0);
        ~OSMDirectoryPathDialog();

        void setOSMDirectoryPath(QString path);
        QString getOSMDirectoryPath();

    private slots:
        void resetToDefault();
        void selectDirectory();

    private:
        Ui::OSMDirectoryPathDialog *ui = nullptr;
};

#endif // OSMDIRECTORYPATHDIALOG_H
