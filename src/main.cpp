#include "MainWindow.h"
#include <QApplication>
#include "hmi/AreaGeometry.h"

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    MainWindow *w = MainWindow::getInstance();

    qRegisterMetaType<AreaGeometry>("SelectGeometry");

    w->initialize();
    w->show();

    int retval = a.exec();

    delete w;

    return retval;
}
