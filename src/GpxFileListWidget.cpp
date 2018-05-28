#include "GpxFileListWidget.h"
#include "ui_GpxFileListWidget.h"

#include <QFileDialog>
#include <QTableView>
#include <QPushButton>

GpxFileListWidget::GpxFileListWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxFileListWidget),
    _gpxManager(gpxManager),
    _gpxLayer(gpxLayer)
{
    _ui->setupUi(this);

    _tableModel = new GpxTableListModel(this);

    _ui->tableView->setModel(_tableModel);

    QObject::connect(_ui->addFile, SIGNAL(clicked(bool)), SLOT(addFile()));
    QObject::connect(_ui->deleteFile, SIGNAL(clicked(bool)), SLOT(deleteFile()));
    QObject::connect(_ui->deleteAllFile, SIGNAL(clicked(bool)), SLOT(deleteAllFile()));
    QObject::connect(_ui->clearSelection, SIGNAL(clicked(bool)), SLOT(clearSelection()));
    QObject::connect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    _ui->deleteFile->setDisabled(true);
    _ui->deleteAllFile->setDisabled(true);
    _ui->clearSelection->setDisabled(true);

    reloadGpx();
}

GpxFileListWidget::~GpxFileListWidget()
{
    delete _ui;
}

void GpxFileListWidget::addFile()
{
    if(_gpxManager != nullptr)
    {
        QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Select GPX Files"), _gpxManager->getLastPathToGpxFiles(), tr("GPX (*.gpx)"));

        _gpxManager->loadGpxFiles(filePaths);

        reloadGpx();
    }
}

int GpxFileListWidget::getId(const QModelIndex & index)
{
    if(index.isValid() == true)
    {
        int fileNameColumnIndex = _tableModel->getColumnIndex(GpxTableListModel::HeaderTableEnum::FileName);

        if(fileNameColumnIndex != GpxTableListModel::ERROR_INDEX)
        {
            QString stringId = _tableModel->data(index, Qt::UserRole + 1).toString();

            if(stringId.isEmpty() == false)
            {
                bool isOk = false;
                int id = stringId.toInt(&isOk);

                if(isOk == true)
                {
                    return id;
                }
            }
        }
    }

    return GpxManager::ErrorId;
}

void GpxFileListWidget::deleteFile()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();
        QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();

        for(const QModelIndex & index : indexes)
        {
            int id = getId(index);

            if(id != GpxManager::ErrorId)
            {
                for(const GpxManager::GpxItem & gpxItem : gpxVector)
                {
                    if(gpxItem.fileId == id)
                    {
                        _gpxManager->removeGpxFile(id);
                    }
                }
            }
        }
    }

    reloadGpx();
}

void GpxFileListWidget::deleteAllFile()
{
    if(_gpxManager != nullptr)
    {
        _gpxManager->removeAll();
    }

    reloadGpx();
}

void GpxFileListWidget::reloadGpx()
{
    if(_gpxManager != nullptr)
    {
        _tableModel->clear();

        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & gpxItem : gpxVector)
        {
            QFileInfo fileInfo(gpxItem.filePath);
            GpxTableListModel::TableItem tableItem;

            tableItem.fileName = fileInfo.fileName();
            tableItem.fileId = gpxItem.fileId;
            tableItem.description = gpxItem.description;
            tableItem.name = gpxItem.name;
            tableItem.authorName = gpxItem.authorName;

            if(gpxItem.startTime.isNull() == false)
            {
                QDateTime time = gpxItem.startTime.toDateTime();

                tableItem.startTime = time.toString("dd.MM.yyyy HH:mm");
            }
            else
            {
                tableItem.startTime = "--";
            }

            if(gpxItem.pointVector.size() > 0)
            {
                time_t tripTime = 0;

                if(gpxItem.pointVector[0].time.isNull() == false && gpxItem.pointVector[gpxItem.pointVector.size() - 1].time.isNull() == false)
                {
                    QDateTime firstTime = gpxItem.pointVector[0].time.toDateTime();
                    QDateTime lastTime = gpxItem.pointVector[gpxItem.pointVector.size() - 1].time.toDateTime();

                    tripTime = lastTime.toTime_t() - firstTime.toTime_t();

                    int hour = int(tripTime / 3600);
                    tripTime -= hour * 3600;
                    int min = int(tripTime / 60);
                    tripTime -= min * 60;
                    int sec = tripTime;

                    tableItem.tripTime = QString("%1:%2:%3").arg(hour).arg(min, 2,'f', 0,'0').arg(sec, 2,'f', 0,'0');;
                }
            }

            _tableModel->addNewItem(tableItem);
        }

        _ui->deleteAllFile->setDisabled(gpxVector.empty());
        _ui->deleteFile->setDisabled(true);
        _ui->clearSelection->setDisabled(true);
    }

    _ui->tableView->resizeColumnsToContents();
}

void GpxFileListWidget::selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    _ui->deleteFile->setDisabled(selected.isEmpty());
    _ui->clearSelection->setDisabled(selected.isEmpty());

    QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();
    GpxManager::GpxIdVector selectedGpx;

    for(const QModelIndex & index : indexes)
    {
        int id = getId(index);

        if(id != GpxManager::ErrorId)
        {
            selectedGpx.push_back(id);
        }
    }

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->setCurrentGpxIndexes(selectedGpx);
    }
}

void GpxFileListWidget::clearSelection()
{
    _ui->tableView->selectionModel()->clear();
}
