#ifndef CENTERPOINTEDITDIALOG_H
#define CENTERPOINTEDITDIALOG_H

#include <QDialog>

#include "CenterPointStruct.h"

namespace Ui {
class CenterPointEditDialog;
}

class CenterPointEditDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit CenterPointEditDialog(const CenterPointStruct & point, QWidget *parent = 0);
        ~CenterPointEditDialog();

    private:
        Ui::CenterPointEditDialog * _ui;
};

#endif // CENTERPOINTEDITDIALOG_H
