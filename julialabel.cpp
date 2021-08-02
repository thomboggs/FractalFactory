#include "julialabel.h"
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

JuliaLabel::JuliaLabel(QWidget *parent) : QLabel(parent)
{
    this->resize(800, 800);
    this->juliaImage = new QImage(width(), height(), QImage::Format_RGB32);
    this->calcImage(QPointF(0.0,0.0));
}

void JuliaLabel::calcImage(QPointF zPoint)
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

int JuliaLabel::calcJulia(QPointF zPoint, QPoint cPoint)
{
//    std::complex<double> complexPoint(4*((float)cPoint.x() / width()) - 2,
//                                      4*((float)cPoint.y() / height()) - 2);
//    std::complex<double> complexZ(zPoint.x(), zPoint.y());
    std::complex<double> complexZ(4*((float)cPoint.x() / width()) - 2,
                                      4*((float)cPoint.y() / height()) - 2);
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

void JuliaLabel::paintEvent(QPaintEvent *)
{
    QPainter oPainter(this);
    oPainter.setRenderHint(QPainter::Antialiasing);

    oPainter.drawImage(0, 0, *(this->juliaImage));
}

QImage* JuliaLabel::getImage()
{
    return juliaImage;
}

void JuliaLabel::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);
//    this->mCoord.setText(QString('new Coord'))
    qDebug() << xpos << ypos;
    qDebug() << mathCoord[0] << mathCoord[1];

    // Calc new julia set fractal and save
    this->calcImage(QPointF(mathCoord[0], mathCoord[1]));
    update();
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}

QPoint JuliaLabel::getDispCoord(double ptX, double ptY) {
    // Assume plot limits as +-2 in both directions
    // ptXY is 0 to width()
    // ... ptXY val is 4*(ptXY-width())-2

    double xDisp = width()*((ptX+2)/4);
    double yDisp = width()*((ptY+2)/4);
    int xDisp_int = std::round(xDisp);
    int yDisp_int = std::round(yDisp);

//    qDebug() << "Double Disp: "  << QString::number(xDisp) << QString::number(yDisp);
    QPoint output = QPoint(xDisp_int, yDisp_int);
//    qDebug() << QString::number(output.x()) << QString::number(output.y());
    return output;
}

void JuliaLabel::recieveBrotCoord(QPointF clickCoord)
{
    // The fucntion recieves the coordinates clicked in mandelbrot, and renders new Julia Set
    // clickCoord is MathCoord
    // Calc new julia set fractal and save
    this->calcImage(clickCoord);
    update();
}

std::vector<double> JuliaLabel::getMathCoord(int ptX, int ptY)
{
    double xMath = 4*((double)ptX / width()) - 2;
    double yMath = 4*((double)ptY / height()) - 2;
    std::vector output = {xMath, yMath};
//    qDebug() << QString::number(output[0]) << QString::number(output[1]);
    return output;
}

void JuliaLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);

    // Calc new julia set fractal and save
    this->calcImage(QPointF(mathCoord[0], mathCoord[1]));
    update();
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}
