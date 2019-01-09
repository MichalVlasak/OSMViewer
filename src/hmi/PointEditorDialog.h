#ifndef POINTEDITORDIALOG_H
#define POINTEDITORDIALOG_H

#include <QDialog>

namespace Ui {
class PointEditorDialog;
}

class PointEditorDialog : public QDialog
{
        Q_OBJECT

    public:
        PointEditorDialog(const QPointF & point, QWidget *parent = nullptr);
        ~PointEditorDialog();

        QPointF getCurrentPosition() const;

    private:
        Ui::PointEditorDialog * _ui = nullptr;
};

#endif // POINTEDITORDIALOG_H
