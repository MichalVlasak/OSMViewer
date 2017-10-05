#include "CenterPointsWidget.h"
#include "ui_CenterPointsWidget.h"

#include "MainWindow.h"
#include "CenterPointEditDialog.h"

CenterPointsWidget::CenterPointsWidget(CenterPointsManager & pointsManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::CenterPointsWidget),
    _pointsManager(pointsManager)
{
    _ui->setupUi(this);

    _ui->homeName->setText(_pointsManager.getHomeCenterPoint().name);

    QObject::connect(_ui->centerHomeButton, SIGNAL(clicked(bool)), SLOT(centerHome()));
    QObject::connect(_ui->editHomeButton, SIGNAL(clicked(bool)), SLOT(editHome()));
}

CenterPointsWidget::~CenterPointsWidget()
{
    delete _ui;
}

void CenterPointsWidget::centerHome()
{
    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        mainWindow->centerToPoint(_pointsManager.getHomeCenterPoint());
    }
}

void CenterPointsWidget::editHome()
{
    CenterPointEditDialog * dialog = new CenterPointEditDialog(_pointsManager.getHomeCenterPoint(), this);

    if(dialog->exec() == QDialog::Accepted)
    {
    }
}
