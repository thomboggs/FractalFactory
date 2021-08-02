#include "mainwindow.h"
#include "mandelbrot.h"
#include "julia.h"
#include "testWidget.h"
#include "mandelbrotlabel.h"
#include "julialabel.h"

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QPixmap>
#include <QImage>
#include <QGridLayout>
#include <QLayout>
#include <QGroupBox>


MainWindow::MainWindow()
{
    setWindowTitle(tr("Mandelbrot Orbits and Julia Sets"));
//    this->resize(950, 500);

    MandelbrotLabel* brot = new MandelbrotLabel(this);
    QImage* brotImage = brot->getImage();
    JuliaLabel* julia = new JuliaLabel(this);
    QImage* juliaImage = julia->getImage();

//    QLabel *testMan = new QLabel(tr("Mandelbrot"), this);
//    QLabel *testJul = new QLabel(tr("Julia"), this);
//    QPixmap testPix1 = QPixmap(400, 400);
//    testPix1.fill(Qt::red);
//    QPixmap testPix2 = QPixmap(400, 400);
//    testPix2.fill(Qt::blue);

    brot->setPixmap(QPixmap::fromImage(*brotImage));
//    testJul->setPixmap(testPix2);
    julia->setPixmap(QPixmap::fromImage(*juliaImage));

    QLabel *mandelbrotLabel = new QLabel(tr("Mandelbrot Set with Orbits"));
    mandelbrotLabel->setAlignment(Qt::AlignHCenter);
    QLabel *juliaLabel = new QLabel(tr("Julia Sets"));
    juliaLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(brot, 0, 0);
    layout->addWidget(julia, 0, 1);
    layout->addWidget(mandelbrotLabel, 1, 0);
    layout->addWidget(juliaLabel, 1, 1);
    this->setLayout(layout);

    connect(brot, &MandelbrotLabel::sendMouseCoord, julia, &JuliaLabel::recieveBrotCoord);
    connect(julia, &JuliaLabel::sendMouseCoord, brot, &MandelbrotLabel::recieveJuliaCoord);

//    qDebug() << MandelbrotLabel()

}

MainWindow::~MainWindow()
{
}

/*
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

//    qDebug() << xpos << ypos;
}
*/


