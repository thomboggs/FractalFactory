#include "juliaworker.h"
#include <complex>


JuliaWorker::JuliaWorker(QObject *parent) :
    QObject(parent)
{
    // todo: need to make this scoped (RAII)
//    this->juliaImage = new QImage(xsize, ysize, QImage::Format_RGB32);
}

JuliaWorker::~JuliaWorker()
{

}

void JuliaWorker::calcImage(QPointF zPoint)
{
    // This Thread calculates the Updated Filled Julia Set Image
    QRgb value;

    // Calculate Julia
    int pixelColorValue = 0;

    // Iterate Through pixels
    for (int ptX = 0; ptX < xsize; ptX++)
    {
        for (int ptY = 0; ptY < ysize; ptY++)
        {
            pixelColorValue = calcJulia(zPoint, QPoint(static_cast<double>(ptX),
                                        static_cast<double>(ptY)));
            value = qRgb(pixelColorValue, 0, 0);
            this->juliaImage->setPixel(ptX, ptY, value);
        }
    }

    // Move the image to the parent object
    emit sendImage(this->juliaImage);
    emit finished();
}

int JuliaWorker::calcJulia(QPointF zPoint, QPoint cPoint)
{
    std::complex<double> complexZ(4*((float)cPoint.x() / xsize) - 2,
                                      4*((float)cPoint.y() / ysize) - 2);
    std::complex<double> complexPoint(zPoint.x(), zPoint.y());

    int nIterations = 3;
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
