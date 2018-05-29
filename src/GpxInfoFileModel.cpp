#include "GpxInfoFileModel.h"

GpxInfoFileModel::GpxInfoFileModel(QObject * parent)
    : QStandardItemModel(parent)
{
    _headerMap[HeaderTableEnum::Time]      = {0, QObject::tr("Time")};
    _headerMap[HeaderTableEnum::Elevation] = {1, QObject::tr("Elevation")};
    _headerMap[HeaderTableEnum::HeartRate] = {2, QObject::tr("Heart Rate")};
    _headerMap[HeaderTableEnum::Cadention] = {3, QObject::tr("Cadention")};
    _headerMap[HeaderTableEnum::Temperature] = {4, QObject::tr("Temperature")};
    _headerMap[HeaderTableEnum::Latitude]  = {5, QObject::tr("Latitude")};
    _headerMap[HeaderTableEnum::Longitude] = {6, QObject::tr("Longitude")};

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

void GpxInfoFileModel::setItemPoints(const GpxManager::PointVector & points)
{
    clear();

    for(const GpxManager::Point & point : points)
    {
        insertRow(rowCount());

        int row = rowCount() - 1;
        QModelIndex index;

        if(_headerMap.find(HeaderTableEnum::Time) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Time].col);

            if(point.time.isNull() == false)
            {
                QDateTime time = point.time.toDateTime();

                setData(index, time.toString("HH:mm:ss"));
            }
        }

        if(_headerMap.find(HeaderTableEnum::Latitude) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Latitude].col);
            setData(index, point.lat);
        }

        if(_headerMap.find(HeaderTableEnum::Longitude) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Longitude].col);
            setData(index, point.lon);
        }

        if(_headerMap.find(HeaderTableEnum::Elevation) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Elevation].col);
            setData(index, point.elevation);
        }

        if(_headerMap.find(HeaderTableEnum::HeartRate) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::HeartRate].col);
            setData(index, point.heartRate);
        }

        if(_headerMap.find(HeaderTableEnum::Cadention) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Cadention].col);
            setData(index, point.cadention);
        }

        if(_headerMap.find(HeaderTableEnum::Temperature) != _headerMap.end())
        {
            index = this->index(row, _headerMap[HeaderTableEnum::Temperature].col);
            setData(index, point.temperature);
        }
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
