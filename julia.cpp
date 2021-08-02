#include "julia.h"
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


Julia::Julia(QWidget *parent)
{
    this->resize(400, 400);
    this->juliaImage = new QImage(width(), height(), QImage::Format_RGB32);
    this->calcImage(QPointF(0.0,0.0));
}

Julia::~Julia()
{

}

void Julia::calcImage(QPointF zPoint)
{
    // This Function calculates the fractal, and then saves it on the
    // .. heap for quick painting.

    QRgb value;

    // Calculate Julia
    int pixelColorValue = 0;

    for (int ptX = 0; ptX < width(); ptX++)
    {
        for (int ptY = 0; ptY < height(); ptY++)
        {
            pixelColorValue = calcJulia(zPoint, QPoint(static_cast<double>(ptX),
                                        static_cast<double>(ptY)));
            value = qRgb(pixelColorValue, 0, 0);
            this->juliaImage->setPixel(ptX, ptY, value);
        }
    }
}

int Julia::calcJulia(QPointF zPoint, QPoint cPoint)
{
    std::complex<double> complexPoint(4*((float)cPoint.x() / width()) - 2,
                                      4*((float)cPoint.y() / height()) - 2);
    std::complex<double> complexZ(zPoint.x(), zPoint.y());

    int nIterations = 0;

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

void Julia::paintEvent(QPaintEvent *)
{

}

QImage* Julia::getImage()
{
    return juliaImage;
}
