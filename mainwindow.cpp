#include "mainwindow.h"
#include "mandelbrot.h"
#include "julia.h"
#include "juliaworker.h"

#include <QLabel>
//#include <QMouseEvent>
//#include <QDebug>
//#include <vector>
//#include <QPixmap>
#include <QImage>
#include <QGridLayout>
#include <QLayout>
#include <QGroupBox>
//#include <QThread>


MainWindow::MainWindow()
{
    setWindowTitle(tr("Mandelbrot Orbits and Julia Sets"));
    // Todo: add menubar with info about app  and potentially saving pics

    Mandelbrot* brot = new Mandelbrot(this);
    QImage* brotImage = brot->getImage();
    Julia* julia = new Julia(this);
    QImage* juliaImage = julia->getImage();

    brot->setPixmap(QPixmap::fromImage(*brotImage));
    julia->setPixmap(QPixmap::fromImage(*juliaImage));

    QLabel *mandelbrotLabel = new QLabel(tr("Mandelbrot Set with Orbits"), this);
    mandelbrotLabel->setAlignment(Qt::AlignHCenter);
    QLabel *juliaLabel = new QLabel(tr("Julia Sets"), this);
    juliaLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(brot, 0, 0);
    layout->addWidget(julia, 0, 1);
    layout->addWidget(mandelbrotLabel, 1, 0);
    layout->addWidget(juliaLabel, 1, 1);
    this->setLayout(layout);

    connect(brot, &Mandelbrot::sendMouseCoord,
            julia, &Julia::recieveBrotCoord);
    connect(julia, &Julia::sendMouseCoord,
            brot, &Mandelbrot::recieveJuliaCoord);
}

MainWindow::~MainWindow()
{
    // When a QObject is deleted, all of its children are automatically deleted.
    // .. All objects created in the mainWindow constructor are children, and
    // .. automatically deleted.
}
