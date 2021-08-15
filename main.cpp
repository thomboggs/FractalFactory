#include "mainwindow.h"

#include <QApplication>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QImage*>(); // This allows a QImage* to be passed into signal/slot
    MainWindow w;
    w.show();

    return a.exec();
}
