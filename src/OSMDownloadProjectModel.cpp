#include "OSMDownloadProjectModel.h"
#include "AppSettings.h"

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

    if(project.setup.geometry.geometryType == SelectGeometry::Type::Rectangle &&
       project.setup.geometry.geometry.isNull() == false &&
       project.setup.geometry.geometry.canConvert<QRectF>() == true)
    {
        QRectF geometryRect = project.setup.geometry.geometry.toRectF();

        double lonFrom = geometryRect.topLeft().x();
        double latFrom = geometryRect.topLeft().y();
        double lonTo = geometryRect.bottomRight().x();
        double latTo = geometryRect.bottomRight().y();

        QDomElement rectangleElement = doc.createElement("Rectangle");
        element.appendChild(rectangleElement);

        QDomElement projectLatitudeFromElement = doc.createElement("LatitudeFrom");
        rectangleElement.appendChild(projectLatitudeFromElement);
        QDomText projectLatitudeFromText = doc.createTextNode(QString::number(latFrom, 'g', 13));
        projectLatitudeFromElement.appendChild(projectLatitudeFromText);

        QDomElement projectLongitudeFromElement = doc.createElement("LongitudeFrom");
        rectangleElement.appendChild(projectLongitudeFromElement);
        QDomText projectLongitudeFromText = doc.createTextNode(QString::number(lonFrom, 'g', 13));
        projectLongitudeFromElement.appendChild(projectLongitudeFromText);

        QDomElement projectLatitudeToElement = doc.createElement("LatitudeTo");
        rectangleElement.appendChild(projectLatitudeToElement);
        QDomText projectLatitudeToText = doc.createTextNode(QString::number(latTo, 'g', 13));
        projectLatitudeToElement.appendChild(projectLatitudeToText);

        QDomElement projectLongitudeToElement = doc.createElement("LongitudeTo");
        rectangleElement.appendChild(projectLongitudeToElement);
        QDomText projectLongitudeToText = doc.createTextNode(QString::number(lonTo, 'g', 13));
        projectLongitudeToElement.appendChild(projectLongitudeToText);
    }
}

void OSMDownloadProjectModel::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement projectsElement = document.createElement("Projects");
    rootElement.appendChild(projectsElement);

    for(const OSMDownloadProjectModel::Project & project : _projects)
    {
        QDomElement projectElement = document.createElement("Project");
        projectsElement.appendChild(projectElement);

        OSMDownloadProjectModel::storeProject(project, projectElement, document);
    }
}

bool OSMDownloadProjectModel::restoreConfig(QDomDocument &document)
{
    bool result = true;
    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList projectsNodes = rootElem.elementsByTagName("Projects");

        for(int iProjects = 0; iProjects < projectsNodes.size(); iProjects++)
        {
            QDomNode projectsNode = projectsNodes.at(iProjects);

            if(projectsNode.isNull() == false)
            {
                QDomNodeList projectNodeList = projectsNode.toElement().elementsByTagName("Project");

                for(int iProject = 0; iProject < projectNodeList.size(); iProject++)
                {
                    QDomNode projectNode = projectNodeList.at(iProject);

                    if(projectNode.isNull() == false)
                    {
                        QString projectName = AppSettings::getValueString(projectNode, "Name");
                        QString levelFrom = AppSettings::getValueString(projectNode, "LevelFrom");
                        QString levelTo = AppSettings::getValueString(projectNode, "LevelTo");

                        if(projectName.isEmpty() == false && levelFrom.isEmpty() == false && levelTo.isEmpty() == false)
                        {
                            QDomNodeList rectangleNodeList = projectNode.toElement().elementsByTagName("Rectangle");

                            for(int iRectangle = 0; iRectangle < rectangleNodeList.size(); iRectangle++)
                            {
                                QDomNode rectangleNode = rectangleNodeList.at(iRectangle);

                                if(rectangleNode.isNull() == false)
                                {
                                    QString latFromStr = AppSettings::getValueString(rectangleNode, "LatitudeFrom");
                                    QString lonFromStr = AppSettings::getValueString(rectangleNode, "LongitudeFrom");
                                    QString latToStr = AppSettings::getValueString(rectangleNode, "LatitudeTo");
                                    QString lonToStr = AppSettings::getValueString(rectangleNode, "LongitudeTo");

                                    if(latFromStr.isEmpty() == false && latToStr.isEmpty() == false &&
                                       lonFromStr.isEmpty() == false && lonToStr.isEmpty() == false)
                                    {
                                        OSMDownloadProjectModel::Project project;
                                        bool isOK;

                                        project.name = projectName;
                                        project.setup.levelFrom = levelFrom.toDouble(&isOK);

                                        if(isOK == false) break;

                                        project.setup.levelTo = levelTo.toDouble(&isOK);

                                        if(isOK == false) break;

                                        double latFrom = latFromStr.toDouble(&isOK);

                                        if(isOK == false) break;

                                        double latTo = latToStr.toDouble(&isOK);

                                        if(isOK == false) break;

                                        double lonFrom = lonFromStr.toDouble(&isOK);

                                        if(isOK == false) break;

                                        double lonTo = lonToStr.toDouble(&isOK);

                                        if(isOK == false) break;

                                        project.setup.geometry.geometryType = SelectGeometry::Type::Rectangle;
                                        project.setup.geometry.geometry = QRectF(QPointF(lonFrom, latFrom), QPointF(lonTo, latTo));

                                        result &= addProject(project);

                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}
