#include "GpxWidget.h"
#include "ui_GpxWidget.h"

#include <QFileDialog>

GpxWidget::GpxWidget(GpxManager * gpxManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxWidget),
    _gpxManager(gpxManager)
{
    _ui->setupUi(this);

    _tableModel = new GpxTableModel(this);

    _ui->tableView->setModel(_tableModel);

    QObject::connect(_ui->addFile, SIGNAL(clicked(bool)), SLOT(addFile()));
    QObject::connect(_ui->deleteFile, SIGNAL(clicked(bool)), SLOT(deleteFile()));
    QObject::connect(_ui->deleteAllFile, SIGNAL(clicked(bool)), SLOT(deleteAllFile()));
    QObject::connect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    _ui->deleteFile->setDisabled(true);
    _ui->deleteAllFile->setDisabled(true);

    reloadGpx();
}

GpxWidget::~GpxWidget()
{
    delete _ui;
}

void GpxWidget::addFile()
{
    if(_gpxManager != nullptr)
    {
        QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Select GPX Files"), _gpxManager->getLastPathToGpxFiles(), tr("GPX (*.gpx)"));

        _gpxManager->loadGpxFiles(filePaths);

        reloadGpx();
    }
}

void GpxWidget::deleteFile()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();
        QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();

        for(const QModelIndex & index : indexes)
        {
            if(index.isValid() == true)
            {
                int fileNameColumnIndex = _tableModel->getColumnIndex(GpxTableModel::HeaderTableEnum::FileName);

                if(fileNameColumnIndex != GpxTableModel::ERROR_INDEX)
                {
                    QString stringId = _tableModel->data(index, Qt::UserRole + 1).toString();

                    if(stringId.isEmpty() == false)
                    {
                        bool isOk = false;
                        int id = stringId.toInt(&isOk);

                        if(isOk == true)
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
            }
        }
    }

    reloadGpx();
}

void GpxWidget::deleteAllFile()
{
    if(_gpxManager != nullptr)
    {
        _gpxManager->removeAll();
    }

    reloadGpx();
}

void GpxWidget::reloadGpx()
{
    if(_gpxManager != nullptr)
    {
        _tableModel->clear();

        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & gpxItem : gpxVector)
        {
            QFileInfo fileInfo(gpxItem.filePath);
            GpxTableModel::TableItem tableItem;

            tableItem.fileName = fileInfo.fileName();
            tableItem.fileId = gpxItem.fileId;
            tableItem.description = gpxItem.description;
            tableItem.name = gpxItem.name;
            tableItem.authorName = gpxItem.authorName;

            if(gpxItem.time.isNull() == false)
            {
                QDateTime time = gpxItem.time.toDateTime();

                tableItem.time = time.toString("dd.MM.yyyy HH:mm");
            }
            else
            {
                tableItem.time = "--";
            }

            _tableModel->addNewItem(tableItem);
        }

        _ui->deleteAllFile->setDisabled(gpxVector.empty());
        _ui->deleteFile->setDisabled(true);
    }

    _ui->tableView->resizeColumnsToContents();
}

void GpxWidget::selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    _ui->deleteFile->setDisabled(selected.isEmpty());
}
