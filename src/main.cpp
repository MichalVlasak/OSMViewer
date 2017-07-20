#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = MainWindow::getInstance();
    w->show();

    int retval = a.exec();

    delete w;

    return retval;
}
