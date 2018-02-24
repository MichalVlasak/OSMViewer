#ifndef OSMDOWNLOADPROJECTMODEL_H
#define OSMDOWNLOADPROJECTMODEL_H

#include <QString>
#include <QtXml>
#include <vector>

#include "OSMDownloadAreaDialog.h"

class OSMDownloadProjectModel : public QObject
{
        Q_OBJECT
    public:
        struct Project
        {
                Project(const QString & pName, const OSMDownloadAreaDialog::Setup pSetup)
                    : name(pName), setup(pSetup)
                {
                }

                Project()
                    : name("Unknown")
                {
                }

                QString name;
                OSMDownloadAreaDialog::Setup setup;
        };

        typedef std::vector<Project> ProjectVector;

    public:
        OSMDownloadProjectModel();

    public:
        const ProjectVector & getProjects() const;
        bool addProject(const Project & project);
        bool updateProject(const Project & project);
        void deleteProject(const QString & projectName);

        static void storeProject(const Project & project, QDomElement & element, QDomDocument & doc);

    signals:
        void modelChanged();

    private:
        ProjectVector _projects;

};

#endif // OSMDOWNLOADPROJECTMODEL_H
