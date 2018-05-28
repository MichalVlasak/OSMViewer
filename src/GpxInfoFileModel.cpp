#include "GpxInfoFileModel.h"

GpxInfoFileModel::GpxInfoFileModel(QObject * parent)
    : QStandardItemModel(parent)
{
    _headerMap[HeaderTableEnum::Time]      = {0, QObject::tr("Time")};
    _headerMap[HeaderTableEnum::Latitude]  = {1, QObject::tr("Latitude")};
    _headerMap[HeaderTableEnum::Longitude] = {2, QObject::tr("Longitude")};
    _headerMap[HeaderTableEnum::Elevation] = {3, QObject::tr("Elevation")};
    _headerMap[HeaderTableEnum::HeartRate] = {4, QObject::tr("Heart Rate")};
    _headerMap[HeaderTableEnum::Cadention] = {5, QObject::tr("Cadention")};

    initializeTableHeader();
}

void GpxInfoFileModel::initializeTableHeader()
{
    this->setColumnCount(_headerMap.size());
    this->setRowCount(0);

    for(HeaderMap::iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        setHeaderData((*it).second.col, Qt::Horizontal, (*it).second.colName);
    }
}

void GpxInfoFileModel::addNewItem(const GpxManager::PointVector & points)
{
    insertRow(rowCount());

    int row = rowCount() - 1;
    QModelIndex index;

    if(_headerMap.find(HeaderTableEnum::Time) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Time].col);
        setData(index, "--");
    }

    if(_headerMap.find(HeaderTableEnum::Latitude) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Latitude].col);
        setData(index, "--");
    }

    if(_headerMap.find(HeaderTableEnum::Longitude) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Longitude].col);
        setData(index, "--");
    }

    if(_headerMap.find(HeaderTableEnum::Elevation) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Elevation].col);
        setData(index, "--");
    }

    if(_headerMap.find(HeaderTableEnum::HeartRate) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::HeartRate].col);
        setData(index, "--");
    }

    if(_headerMap.find(HeaderTableEnum::Cadention) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::Cadention].col);
        setData(index, "--");
    }
}

void GpxInfoFileModel::clear()
{
    QStandardItemModel::clear();
    initializeTableHeader();
}

int GpxInfoFileModel::getColumnIndex(HeaderTableEnum headerItem) const
{
    HeaderMap::const_iterator it = _headerMap.find(headerItem);

    if(it != _headerMap.end())
    {
        return it->second.col;
    }

    return ERROR_INDEX;
}
