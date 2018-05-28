#include "GpxFilesListModel.h"

GpxFilesListModel::GpxFilesListModel(QObject *parent)
    : QStandardItemModel(parent)
{
    _headerMap[HeaderTableEnum::FileName]    = {0, QObject::tr("File Name")};
    _headerMap[HeaderTableEnum::StartTime]   = {1, QObject::tr("Start Time")};
    _headerMap[HeaderTableEnum::TripTime]    = {2, QObject::tr("Trip Time")};
    _headerMap[HeaderTableEnum::AuthorName]  = {3, QObject::tr("Author Name")};
    _headerMap[HeaderTableEnum::Name]        = {4, QObject::tr("Name")};
    _headerMap[HeaderTableEnum::Description] = {5, QObject::tr("Description")};

    initializeTableHeader();
}

void GpxFilesListModel::initializeTableHeader()
{
    this->setColumnCount(_headerMap.size());
    this->setRowCount(0);

    for(HeaderMap::iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        setHeaderData((*it).second.col, Qt::Horizontal, (*it).second.colName);
    }
}

void GpxFilesListModel::addNewItem(const GpxManager::GpxItem & item)
{
    insertRow(rowCount());

    int row = rowCount() - 1;
    QModelIndex index;

    if(_headerMap.find(HeaderTableEnum::FileName) != _headerMap.end())
    {
        QFileInfo fileInfo(item.filePath);

        index = this->index(row, _headerMap[HeaderTableEnum::FileName].col);
        setData(index, fileInfo.fileName());
        setData(index, item.fileId, Qt::UserRole + 1); // ako identifikator pouzijem jedinecne generovane ID pre kazdygpx subor
    }

    if(_headerMap.find(HeaderTableEnum::Description) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Description].col);
        setData(index, item.description);
    }

    if(_headerMap.find(HeaderTableEnum::Name) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Name].col);
        setData(index, item.name);
    }

    if(_headerMap.find(HeaderTableEnum::AuthorName) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::AuthorName].col);
        setData(index, item.authorName);
    }

    if(_headerMap.find(HeaderTableEnum::StartTime) != _headerMap.end())
    {
        QString startTime = "--";

        if(item.startTime.isNull() == false)
        {
            QDateTime time = item.startTime.toDateTime();

            startTime = time.toString("dd.MM.yyyy HH:mm");
        }

        index = this->index(row, _headerMap[HeaderTableEnum::StartTime].col);
        setData(index, startTime);
    }

    if(_headerMap.find(HeaderTableEnum::TripTime) != _headerMap.end())
    {
        QString startTimeString = "--";

        if(item.pointVector.size() > 0)
        {
            if(item.pointVector[0].time.isNull() == false && item.pointVector[item.pointVector.size() - 1].time.isNull() == false)
            {
                QDateTime firstTime = item.pointVector[0].time.toDateTime();
                QDateTime lastTime = item.pointVector[item.pointVector.size() - 1].time.toDateTime();

                time_t tripTime = lastTime.toTime_t() - firstTime.toTime_t();

                int hour = int(tripTime / 3600);
                tripTime -= hour * 3600;
                int min = int(tripTime / 60);
                tripTime -= min * 60;
                int sec = tripTime;

                startTimeString = QString("%1:%2:%3").arg(hour).arg(min, 2,'f', 0,'0').arg(sec, 2,'f', 0,'0');;
            }
        }

        index = this->index(row, _headerMap[HeaderTableEnum::TripTime].col);
        setData(index, startTimeString);
    }
}

void GpxFilesListModel::clear()
{
    QStandardItemModel::clear();
    initializeTableHeader();
}

int GpxFilesListModel::getColumnIndex(HeaderTableEnum headerItem) const
{
    HeaderMap::const_iterator it = _headerMap.find(headerItem);

    if(it != _headerMap.end())
    {
        return it->second.col;
    }

    return ERROR_INDEX;
}
