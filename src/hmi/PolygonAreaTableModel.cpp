#include "PolygonAreaTableModel.h"

PolygonAreaTableModel::PolygonAreaTableModel(QObject *parent)
    : QStandardItemModel(parent)
{
    _headerMap[HeaderTableEnum::Latitude]  = {0, QObject::tr("Latitude")};
    _headerMap[HeaderTableEnum::Longitude] = {1, QObject::tr("Longitude")};

    initializeTableHeader();
}

void PolygonAreaTableModel::initializeTableHeader()
{
    this->setColumnCount(_headerMap.size());
    this->setRowCount(0);

    for(HeaderMap::iterator it = _headerMap.begin(); it != _headerMap.end(); ++it)
    {
        setHeaderData((*it).second.col, Qt::Horizontal, (*it).second.colName);
    }
}

void PolygonAreaTableModel::setPolygon(const QPolygonF & polygon)
{
    clear();

    for(const QPointF & point : polygon)
    {
        insertRow(rowCount());

        int row = rowCount() - 1;
        QModelIndex index;

        if(_headerMap.find(HeaderTableEnum::Latitude) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Latitude].col);
            setData(index, point.y());
        }

        if(_headerMap.find(HeaderTableEnum::Longitude) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Longitude].col);
            setData(index, point.x());
        }
    }
}

void PolygonAreaTableModel::clear()
{
    QStandardItemModel::clear();
    initializeTableHeader();
}

int PolygonAreaTableModel::getColumnIndex(HeaderTableEnum headerItem) const
{
    HeaderMap::const_iterator it = _headerMap.find(headerItem);

    if(it != _headerMap.end())
    {
        return it->second.col;
    }

    return ERROR_INDEX;
}
