#include "ProjectNameDialog.h"
#include "ui_ProjectNameDialog.h"

ProjectNameDialog::ProjectNameDialog(const QString & projectName, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ProjectNameDialog)
{
    _ui->setupUi(this);
    _ui->projectName->setText(projectName);
}

ProjectNameDialog::~ProjectNameDialog()
{
    delete _ui;
}

const QString ProjectNameDialog::getProjectName() const
{
    return _ui->projectName->text();
}
