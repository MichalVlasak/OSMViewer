#ifndef PROJECTNAMEDIALOG_H
#define PROJECTNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectNameDialog;
}

class ProjectNameDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ProjectNameDialog(const QString & projectName, QWidget *parent = 0);
        ~ProjectNameDialog();

    public:
        const QString getProjectName() const;

    private:
        Ui::ProjectNameDialog *_ui = nullptr;
};

#endif // PROJECTNAMEDIALOG_H
