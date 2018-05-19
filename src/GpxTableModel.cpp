#include "GpxTableModel.h"

GpxTableModel::GpxTableModel(QObject *parent)
    : QStandardItemModel(parent)
{
    _headerMap[HeaderTableEnum::FileName]    = {0, QObject::tr("File Name")};
    _headerMap[HeaderTableEnum::Description] = {1, QObject::tr("Description")};

    initializeTableHeader();
}

void GpxTableModel::initializeTableHeader()
{
    this->setColumnCount(_headerMap.size());
    this->setRowCount(0);

    for(HeaderMap::iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        setHeaderData((*it).second.col, Qt::Horizontal, (*it).second.colName);
    }
}

void GpxTableModel::addNewItem(const TableItem &item)
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
}

void GpxTableModel::clear()
{
    QStandardItemModel::clear();
    initializeTableHeader();
}

int GpxTableModel::getColumnIndex(HeaderTableEnum headerItem) const
{
    HeaderMap::const_iterator it = _headerMap.find(headerItem);

    if(it != _headerMap.end())
    {
        return it->second.col;
    }

    return ERROR_INDEX;
}
