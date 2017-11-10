#ifndef GROUPEDITDIALOG_H
#define GROUPEDITDIALOG_H

#include <QDialog>

namespace Ui {
class GroupEditDialog;
}

class GroupEditDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit GroupEditDialog(const QString & groupName, QWidget *parent = 0);
        ~GroupEditDialog();

        QString getGroupName() const;

    private:
        Ui::GroupEditDialog * _ui;
};

#endif // GROUPEDITDIALOG_H
