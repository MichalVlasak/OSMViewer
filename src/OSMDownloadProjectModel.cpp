#include "OSMDownloadProjectModel.h"

OSMDownloadProjectModel::OSMDownloadProjectModel()
{
}

const OSMDownloadProjectModel::ProjectVector & OSMDownloadProjectModel::getProjects() const
{
    return _projects;
}

bool OSMDownloadProjectModel::addProject(const Project &project)
{
    bool isPresent = false;

    for(const Project & tmp : _projects)
    {
        if(tmp.name.compare(project.name) == 0)
        {
            isPresent = true;

            break;
        }
    }

    if(isPresent == false)
    {
        _projects.push_back(project);

        emit modelChanged();
    }

    return !isPresent;
}

bool OSMDownloadProjectModel::updateProject(const Project &project)
{
    bool isPresent = false;

    for(Project & tmp : _projects)
    {
        if(tmp.name.compare(project.name) == 0)
        {
            tmp = project;
            isPresent = true;
        }
    }

    return isPresent;
}

void OSMDownloadProjectModel::deleteProject(const QString & projectName)
{
    for(size_t i = 0; i < _projects.size(); i++)
    {
        if(_projects[i].name.compare(projectName) == 0)
        {
            _projects.erase(_projects.begin() + i);

            break;
        }
    }
}

void OSMDownloadProjectModel::storeProject(const Project & project, QDomElement &element, QDomDocument &doc)
{
    QDomElement projectNameElement = doc.createElement("Name");
    element.appendChild(projectNameElement);
    QDomText projectNameText = doc.createTextNode(project.name);
    projectNameElement.appendChild(projectNameText);

    QDomElement projectLevelFromElement = doc.createElement("LevelFrom");
    element.appendChild(projectLevelFromElement);
    QDomText projectLevelFromText = doc.createTextNode(QString::number(project.setup.levelFrom));
    projectLevelFromElement.appendChild(projectLevelFromText);

    QDomElement projectLevelToElement = doc.createElement("LevelTo");
    element.appendChild(projectLevelToElement);
    QDomText projectLevelToText = doc.createTextNode(QString::number(project.setup.levelTo));
    projectLevelToElement.appendChild(projectLevelToText);

    QDomElement projectLatitudeFromElement = doc.createElement("LatitudeFrom");
    element.appendChild(projectLatitudeFromElement);
    QDomText projectLatitudeFromText = doc.createTextNode(QString::number(project.setup.latFrom, 'g', 13));
    projectLatitudeFromElement.appendChild(projectLatitudeFromText);

    QDomElement projectLongitudeFromElement = doc.createElement("LongitudeFrom");
    element.appendChild(projectLongitudeFromElement);
    QDomText projectLongitudeFromText = doc.createTextNode(QString::number(project.setup.lonFrom, 'g', 13));
    projectLongitudeFromElement.appendChild(projectLongitudeFromText);

    QDomElement projectLatitudeToElement = doc.createElement("LatitudeTo");
    element.appendChild(projectLatitudeToElement);
    QDomText projectLatitudeToText = doc.createTextNode(QString::number(project.setup.latTo, 'g', 13));
    projectLatitudeToElement.appendChild(projectLatitudeToText);

    QDomElement projectLongitudeToElement = doc.createElement("LongitudeTo");
    element.appendChild(projectLongitudeToElement);
    QDomText projectLongitudeToText = doc.createTextNode(QString::number(project.setup.lonTo, 'g', 13));
    projectLongitudeToElement.appendChild(projectLongitudeToText);
}
