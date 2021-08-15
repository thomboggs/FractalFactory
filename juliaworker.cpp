#include "juliaworker.h"
#include <complex>
#include <QDebug>
#include <QLabel>


JuliaWorker::JuliaWorker(QWidget *parent) : QObject(parent)
{
    // todo: need to make this scoped (RAII)
    this->_juliaImage = new QImage(_xsize, _ysize, QImage::Format_RGB32);
//    this->_parent = parent;
}

JuliaWorker::~JuliaWorker()
{
    emit finished();
}

void JuliaWorker::process(QPointF zPoint)
{
    // This Thread calculates the Updated Filled Julia Set Image
//    qDebug() << tr("JuliaWorker") << this->_parent << this << zPoint.x() << zPoint.y();

    QRgb value;

    // Calculate Julia
    int pixelColorValue = 0;

    // Iterate Through pixels
    for (int ptX = 0; ptX < _xsize; ptX++)
    {
        for (int ptY = 0; ptY < _ysize; ptY++)
        {
//            qDebug() << tr("JuliaWorker: ") << tr("Pre Pixel-Calc");
            pixelColorValue = calcJulia(zPoint, QPoint(static_cast<double>(ptX),
                                        static_cast<double>(ptY)));
//            qDebug() << tr("JuliaWorker: ") << tr("Post Pixel-Calc");
//            value = qRgb(pixelColorValue, 0, 0);
            if (pixelColorValue == 0) value = qRgb(0,0,0);
            else value = qRgb(255, 255-pixelColorValue, 255-pixelColorValue);
            this->_juliaImage->setPixel(ptX, ptY, value);
//            qDebug() << tr("JuliaWorker: ") << tr("Post Set-Pixel");
        }
    }
//    qDebug() << tr("JuliaWorker: ") << tr("Post Calc");
    QImage* test = new QImage(*(this->_juliaImage));
//    qDebug() << tr("JuliaWorker: ") << tr("Post test image");
    // Move the image to the parent object
    emit sendImage(test);
//    qDebug() << tr("JuliaWorker: ") << tr("Post Emit");
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
