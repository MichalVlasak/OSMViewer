#include "GroupEditDialog.h"
#include "ui_GroupEditDialog.h"

#include <QLineEdit>

GroupEditDialog::GroupEditDialog(const QString & groupName, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::GroupEditDialog)
{
    _ui->setupUi(this);
    _ui->nameLineEdit->setText(groupName);

    if(groupName.isEmpty() == true)
    {
        setWindowTitle("Add New Group");
    }
    else
    {
        setWindowTitle("Edit Group");
    }
}

GroupEditDialog::~GroupEditDialog()
{
    delete _ui;
}

QString GroupEditDialog::getGroupName() const
{
    return _ui->nameLineEdit->text();
}
