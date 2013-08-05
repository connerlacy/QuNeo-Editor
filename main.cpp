#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    #ifdef Q_OS_MAC
    w.setMinimumHeight(678);
    w.setMinimumWidth(1039);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1039);
    #else
    w.setMinimumHeight(678);
    w.setMinimumWidth(1039);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1039);
    #endif

    w.show();

    return a.exec();
}
