#include "mainwindow.h"

#include <QApplication>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication brotApp(argc, argv);
    MainWindow w;
    w.show();

    return brotApp.exec();
}
