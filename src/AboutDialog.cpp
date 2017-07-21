#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include "MainWindow.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);

    QString title = tr("About");
    QString name = tr("OSM");

    if(MainWindow::getInstance() != nullptr)
    {
        name = MainWindow::getInstance()->getApplicationName();
        title += " " + name;
    }

    setWindowTitle(title);

    int day, year, hour, min, sec;
    char month[8];
    QString date, tmp;

    sscanf (__DATE__,"%s %d %d", month, &day, &year);
    sscanf (__TIME__,"%d:%d:%d", &hour, &min, &sec);

    date = tmp.setNum(day) + " ";
    date += tmp.fromUtf8(month) + " ";
    date += tmp.setNum(year) + ", ";
    date += tmp.sprintf("%02d", hour) + ":";
    date += tmp.sprintf("%02d", min) + ":";
    date += tmp.sprintf("%02d", sec);

    _ui->nameLabel->setText(name);
    _ui->authorLabel->setText("Michal Vlasak");
    _ui->eMailLabel->setText("michal.vlasak@gmail.com");
    _ui->dateLabel->setText(date);
}

AboutDialog::~AboutDialog()
{
    delete _ui;
}
