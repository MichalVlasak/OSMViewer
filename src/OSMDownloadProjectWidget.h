#ifndef OSMDOWNLOADPROJECT_H
#define OSMDOWNLOADPROJECT_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class OSMDownloadProjectWidgetForm;
}

class OSMDownloadProjectWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit OSMDownloadProjectWidget(QWidget *parent = 0);
        ~OSMDownloadProjectWidget();

        void retranslate();

    public:
        void initialize();

    private slots:
        void reloadProjects();
        void openProject(QModelIndex index);
        void openProject();
        void deleteProject();
        void createProject();
        void updateProject();

    private:
        void openProject(QString projectName);

    private:
        Ui::OSMDownloadProjectWidgetForm *_ui;
        QStringListModel * _listModel = nullptr;
};

#endif // OSMDOWNLOADPROJECT_H
