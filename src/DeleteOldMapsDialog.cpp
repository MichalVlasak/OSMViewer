#include "DeleteOldMapsDialog.h"
#include "ui_DeleteOldMapsDialog.h"

DeleteOldMapsDialog::DeleteOldMapsDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::DeleteOldMapsDialog)
{
    _ui->setupUi(this);
}

DeleteOldMapsDialog::~DeleteOldMapsDialog()
{
    delete _ui;
}

void DeleteOldMapsDialog::setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings)
{
    _ui->deleteWidget->setDeleteSettings(settings);
}

DeleteOldMapsWidget::DeleteSettings DeleteOldMapsDialog::getDeleteSettings() const
{
    return _ui->deleteWidget->getDeleteSettings();
}

void DeleteOldMapsDialog::setEnabledDeleteAll(bool enable)
{
    _ui->deleteWidget->setEnabledDeleteAll(enable);
}
