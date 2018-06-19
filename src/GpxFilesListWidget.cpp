#include "GpxFilesListWidget.h"
#include "ui_GpxFileListWidget.h"

#include <QFileDialog>
#include <QTableView>
#include <QPushButton>
#include <QProgressBar>

GpxFilesListWidget::GpxFilesListWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxFileListWidget),
    _gpxManager(gpxManager),
    _gpxLayer(gpxLayer)
{
    _ui->setupUi(this);

    _tableModel = new GpxFilesListModel(this);

    _ui->tableView->setModel(_tableModel);

    QObject::connect(_ui->addFile, SIGNAL(clicked(bool)), SLOT(addFile()));
    QObject::connect(_ui->deleteFile, SIGNAL(clicked(bool)), SLOT(deleteFile()));
    QObject::connect(_ui->deleteAllFile, SIGNAL(clicked(bool)), SLOT(deleteAllFile()));
    QObject::connect(_ui->clearSelection, SIGNAL(clicked(bool)), SLOT(clearSelection()));
    QObject::connect(_gpxManager, SIGNAL(gpxWasLoadedSignals(int)), SLOT(gpxWasLoadedSlot(int)));
    QObject::connect(_gpxManager, SIGNAL(gpxStatusLoad(int,int)), SLOT(gpxStatusLoad(int,int)));
    QObject::connect(_gpxManager, SIGNAL(gpxStatusAllLoaded()), SLOT(gpxStatusAllLoaded()));
    QObject::connect(_gpxManager, SIGNAL(gpxCurrentLoadingSignals(QString)), SLOT(gpxCurrentLoadingSignals(QString)));
    QObject::connect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    _ui->deleteFile->setDisabled(true);
    _ui->deleteAllFile->setDisabled(true);
    _ui->clearSelection->setDisabled(true);
    _ui->progresBarWidget->hide();

    reloadGpx();
}

GpxFilesListWidget::~GpxFilesListWidget()
{
    delete _ui;
}

void GpxFilesListWidget::addFile()
{
    if(_gpxManager != nullptr)
    {
        QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Select GPX Files"), _gpxManager->getLastPathToGpxFiles(), tr("GPX (*.gpx)"));

        _gpxManager->loadGpxFiles(filePaths);

        reloadGpx();
    }
}

int GpxFilesListWidget::getId(const QModelIndex & index)
{
    if(index.isValid() == true)
    {
        int fileNameColumnIndex = _tableModel->getColumnIndex(GpxFilesListModel::HeaderTableEnum::FileName);

        if(fileNameColumnIndex != GpxFilesListModel::ERROR_INDEX)
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

void GpxFilesListWidget::deleteFile()
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

                        emit deleteGpxSignal(id);
                    }
                }
            }
        }
    }

    reloadGpx();
}

void GpxFilesListWidget::deleteAllFile()
{
    if(_gpxManager != nullptr)
    {
        _gpxManager->removeAll();
    }

    reloadGpx();

    emit deleteAllSignal();;
}

void GpxFilesListWidget::reloadGpx()
{
    if(_gpxManager != nullptr)
    {
        _tableModel->clear();

        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & gpxItem : gpxVector)
        {
            _tableModel->addNewItem(gpxItem);
        }

        _ui->deleteAllFile->setDisabled(gpxVector.empty());
        _ui->deleteFile->setDisabled(true);
        _ui->clearSelection->setDisabled(true);
    }

    _ui->tableView->resizeColumnsToContents();
}

void GpxFilesListWidget::selectionChanged(QItemSelection selected, QItemSelection deselected)
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

    emit changeSelectedGpsSignal(selectedGpx);
}

void GpxFilesListWidget::clearSelection()
{
    _ui->tableView->selectionModel()->clear();
}

void GpxFilesListWidget::gpxWasLoadedSlot(int fileId)
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & gpxItem : gpxVector)
        {
            if(fileId == gpxItem.fileId)
            {
                _tableModel->addNewItem(gpxItem);
            }
        }

        _ui->deleteAllFile->setDisabled(gpxVector.empty());
        _ui->deleteFile->setDisabled(true);
        _ui->clearSelection->setDisabled(true);
    }

    _ui->tableView->resizeColumnsToContents();
}

void GpxFilesListWidget::gpxStatusLoad(int allCount, int currentCounter)
{
    _ui->progresBarWidget->show();
    _ui->progressBar->setMaximum(allCount);
    _ui->progressBar->setValue(currentCounter);
}

void GpxFilesListWidget::gpxStatusAllLoaded()
{
    _ui->progresBarWidget->hide();
}

void GpxFilesListWidget::gpxCurrentLoadingSignals(QString filePath)
{
    QFileInfo fi(filePath);

    _ui->currentLoadFileName->setText(fi.fileName());
}
