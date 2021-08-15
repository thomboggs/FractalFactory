#include "julia.h"
#include "juliaworker.h"
#include <complex>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
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
    _mutex.lock();
    this->_juliaImage = QSharedPointer<QImage>(new QImage(800, 800, QImage::Format_RGB32));
    _juliaImage->fill(Qt::black);
    _mutex.unlock();

    // Add Thread For Julia Calculation
    QThread* thread = new QThread;
    JuliaWorker* worker = new JuliaWorker(this->_juliaImage, _mutex);
    worker->moveToThread(thread);
    connect(this, &Julia::calcJuliaSet,
            worker, &JuliaWorker::process);
    connect(worker, &JuliaWorker::calcComplete,
            this, &Julia::recieveWorkerData);
    connect(worker, &JuliaWorker::finished,
            thread, &QThread::quit);
    connect(worker, &JuliaWorker::finished,
            worker, &JuliaWorker::deleteLater);
    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);
    thread->start();

    update();

    // Create initial Julia Set at (0,0)
    emit this->calcJuliaSet(QPointF(0.0, 0.0));
}

QSharedPointer<QImage> Julia::getImage()
{
    return this->_juliaImage;
}

QPointF Julia::getMathCoord(int ptX, int ptY)
{
    double xMath = 4*((double)ptX / width()) - 2;
    double yMath = 4*((double)ptY / height()) - 2;
    QPointF output = QPointF(xMath, yMath);
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

    // Calc new julia set fractal
    emit calcJuliaSet(clickCoord);
}

void Julia::recieveWorkerData()
{
    update();
}

// Protected
void Julia::paintEvent(QPaintEvent *)
{
//    QMutexLocker locker(&_mutex);
    QPainter oPainter(this);
    oPainter.setRenderHint(QPainter::Antialiasing);

    oPainter.drawImage(0, 0, *(this->_juliaImage));

}

void Julia::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal and save
    emit calcJuliaSet(mathCoord);
    emit sendMouseCoord(mathCoord);
}

void Julia::mouseMoveEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal
    emit sendMouseCoord(mathCoord);
}

void Julia::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal
    emit calcJuliaSet(mathCoord);
}
