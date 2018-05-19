#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    MainWindow *w = MainWindow::getInstance();
    w->initialize();
    w->show();

    int retval = a.exec();

    delete w;

    return retval;
}
