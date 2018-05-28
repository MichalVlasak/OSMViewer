#include "GpxTableListModel.h"

GpxTableListModel::GpxTableListModel(QObject *parent)
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

void GpxTableListModel::initializeTableHeader()
{
    this->setColumnCount(_headerMap.size());
    this->setRowCount(0);

    for(HeaderMap::iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        setHeaderData((*it).second.col, Qt::Horizontal, (*it).second.colName);
    }
}

void GpxTableListModel::addNewItem(const TableItem &item)
{
    insertRow(rowCount());

    int row = rowCount() - 1;
    QModelIndex index;

    if(_headerMap.find(HeaderTableEnum::FileName) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::FileName].col);
        setData(index, item.fileName);
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
        index = this->index(row, _headerMap[HeaderTableEnum::StartTime].col);
        setData(index, item.startTime);
    }

    if(_headerMap.find(HeaderTableEnum::TripTime) != _headerMap.end())
    {
        index = this->index(row, _headerMap[HeaderTableEnum::TripTime].col);
        setData(index, item.tripTime);
    }
}

void GpxTableListModel::clear()
{
    QStandardItemModel::clear();
    initializeTableHeader();
}

int GpxTableListModel::getColumnIndex(HeaderTableEnum headerItem) const
{
    HeaderMap::const_iterator it = _headerMap.find(headerItem);

    if(it != _headerMap.end())
    {
        return it->second.col;
    }

    return ERROR_INDEX;
}
