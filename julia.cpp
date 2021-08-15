#include "julia.h"
#include "juliaworker.h"
#include <complex>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QPixmap>
#include <QImage>
#include <QPointF>
#include <QPoint>
#include <QPainter>
#include <QThread>


// Public Functions

// Constructor
Julia::Julia(QWidget *parent) : QLabel(parent)
{
    this->resize(800, 800);
    this->_juliaImage = new QImage(width(), height(), QImage::Format_RGB32);

    // Add Thread For Julia Calculation
    QThread* thread = new QThread;
    JuliaWorker* worker = new JuliaWorker();
    worker->moveToThread(thread);
    connect(this, &Julia::calcJuliaSet,
            worker, &JuliaWorker::process);
    connect(worker, &JuliaWorker::sendImage,
            this, &Julia::recieveWorkerData);
    connect(worker, &JuliaWorker::finished,
            thread, &QThread::quit);
    connect(worker, &JuliaWorker::finished,
            worker, &JuliaWorker::deleteLater);
    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);
    thread->start();

    // Create initial Julia Set at (0,0)
    emit this->calcJuliaSet(QPointF(0.0, 0.0));

}

void Julia::setImage(QImage* newImage)
{
    this->_juliaImage = newImage;
    update();
}

QImage* Julia::getImage()
{
    return this->_juliaImage;
}

std::vector<double> Julia::getMathCoord(int ptX, int ptY)
{
    double xMath = 4*((double)ptX / width()) - 2;
    double yMath = 4*((double)ptY / height()) - 2;
    std::vector output = {xMath, yMath};
    return output;
}

QPoint Julia::getDispCoord(double ptX, double ptY) {
    // Assume plot limits as +-2 in both directions
    // ptXY is 0 to width()
    // ... ptXY val is 4*(ptXY-width())-2

    double xDisp = width()*((ptX+2)/4);
    double yDisp = width()*((ptY+2)/4);
    int xDisp_int = std::round(xDisp);
    int yDisp_int = std::round(yDisp);

    QPoint output = QPoint(xDisp_int, yDisp_int);

    return output;
}

// Slots
void Julia::recieveBrotCoord(QPointF clickCoord)
{
    // The fucntion recieves the coordinates clicked in mandelbrot, and renders new Julia Set
    // clickCoord is MathCoord
    // Calc new julia set fractal and save

    emit calcJuliaSet(clickCoord);
}

void Julia::recieveWorkerData(QImage* juliaImage)
{
    this->_juliaImage = juliaImage;
    update();
}

// Protected
void Julia::paintEvent(QPaintEvent *)
{
    QPainter oPainter(this);
    oPainter.setRenderHint(QPainter::Antialiasing);

    oPainter.drawImage(0, 0, *(this->_juliaImage));
}

void Julia::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);
    qDebug() << xpos << ypos;
    qDebug() << mathCoord[0] << mathCoord[1];

    // Calc new julia set fractal and save
    emit calcJuliaSet(QPointF(mathCoord[0], mathCoord[1]));
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}

void Julia::mouseMoveEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}

void Julia::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal

    emit calcJuliaSet(QPointF(mathCoord[0], mathCoord[1]));
}
