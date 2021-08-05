#include "juliaworker.h"
#include <complex>
#include <QDebug>


JuliaWorker::JuliaWorker(QWidget *parent)
{
    // todo: need to make this scoped (RAII)
    this->juliaImage = new QImage(xsize, ysize, QImage::Format_RGB32);
    this->_parent = parent;
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
    for (int ptX = 0; ptX < xsize; ptX++)
    {
        for (int ptY = 0; ptY < ysize; ptY++)
        {
//            qDebug() << tr("JuliaWorker: ") << tr("Pre Pixel-Calc");
            pixelColorValue = calcJulia(zPoint, QPoint(static_cast<double>(ptX),
                                        static_cast<double>(ptY)));
//            qDebug() << tr("JuliaWorker: ") << tr("Post Pixel-Calc");
            value = qRgb(pixelColorValue, 0, 0);
            if (pixelColorValue == 0) value = qRgb(0,0,0);
            else value = qRgb(255, 255-pixelColorValue, 255-pixelColorValue);
            this->juliaImage->setPixel(ptX, ptY, value);
//            qDebug() << tr("JuliaWorker: ") << tr("Post Set-Pixel");
        }
    }
//    qDebug() << tr("JuliaWorker: ") << tr("Post Calc");
    QImage* test = new QImage(*(this->juliaImage));
//    qDebug() << tr("JuliaWorker: ") << tr("Post test image");
    // Move the image to the parent object
    emit sendImage(test);
//    qDebug() << tr("JuliaWorker: ") << tr("Post Emit");
}

int JuliaWorker::calcJulia(QPointF zPoint, QPoint cPoint)
{
    std::complex<double> complexZ(4*((float)cPoint.x() / xsize) - 2,
                                      4*((float)cPoint.y() / ysize) - 2);
    std::complex<double> complexPoint(zPoint.x(), zPoint.y());

    int nIterations = _brightness;
    while ((abs(complexZ) < 2 ) && ( nIterations <= maxIterations ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < maxIterations)
        return static_cast<int>(( 255 * nIterations ) / maxIterations );
    else
        return 0;
}
