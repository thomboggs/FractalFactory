#include "juliaworker.h"
#include <complex>
#include <QDebug>
#include <QLabel>


JuliaWorker::JuliaWorker(QSharedPointer<QImage> juliaImage, QMutex& mtx) : _juliaImage(juliaImage) , _mutex(mtx)
{
}

JuliaWorker::~JuliaWorker()
{
//    emit finished();
}

void JuliaWorker::process(QPointF zPoint)
{
    // This Thread calculates the Updated Filled Julia Set Image
    QMutexLocker locker(&_mutex);
    QRgb value;

    // Calculate Julia
    int pixelColorValue = 0;

    // Iterate Through pixels
    for (int ptX = 0; ptX < _xsize; ptX++)
    {
        for (int ptY = 0; ptY < _ysize; ptY++)
        {
            pixelColorValue = calcJulia(zPoint, QPoint(static_cast<double>(ptX),
                                        static_cast<double>(ptY)));
            if (pixelColorValue == 0) value = qRgb(0,0,0); // Color is black if within the set
            else value = qRgb(255, 255-pixelColorValue, 255-pixelColorValue); // Color is White->red depending on num,ber of iterations required to escape
            this->_juliaImage->setPixel(ptX, ptY, value);
        }
    }
    emit calcComplete();
    emit finished();
}

int JuliaWorker::calcJulia(QPointF zPoint, QPoint cPoint)
{
    std::complex<double> complexZ(4*((float)cPoint.x() / _xsize) - 2,
                                      4*((float)cPoint.y() / _ysize) - 2);
    std::complex<double> complexPoint(zPoint.x(), zPoint.y());

    int nIterations = _brightness;
    while ((abs(complexZ) < 2 ) && ( nIterations <= _maxIterations ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < _maxIterations)
        return static_cast<int>(( 255 * nIterations ) / _maxIterations );
    else
        return 0;
}
