#include "mandelbrot.h"
#include <complex>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QColor>


Mandelbrot::Mandelbrot(QWidget *parent) : QLabel(parent)
{
    this->resize(800, 800);

    // Initialize the image data on the heap
    this->_brotImage = QSharedPointer<QImage>(new QImage(width(), height(), QImage::Format_RGB32));

    this->calcMandelbrot();
}

QPoint Mandelbrot::getDispCoord(double ptX, double ptY) {
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

QPointF Mandelbrot::getMathCoord(int ptX, int ptY)
{
    double xMath = 4*((double)ptX / width()) - 2;
    double yMath = 4*((double)ptY / height()) - 2;
    QPointF output = QPointF(xMath, yMath);
    return output;
}

int Mandelbrot::calcIterations(double ptX, double ptY)
{
    std::complex<double> complexPoint(4*(ptX / width()) - 2,
                                      4*(ptY / height()) - 2);
    std::complex<double> complexZ(0, 0);

    int nIterations = _minIterations;

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

std::vector<QPoint> Mandelbrot::calcOrbit(QPointF coordinate) {
    //  Do the same calculation as get color value
    std::complex<double> complexPoint(coordinate.x(), coordinate.y());
    std::complex<double> complexZ(0, 0);

    QPoint currentPoint = this->getDispCoord(coordinate.x(), coordinate.y());
    std::vector<QPoint> orbitVec = {currentPoint};

    int currentstep = 0;
    while ((abs(complexZ) < 2 ) && ( currentstep <= _maxOrbitLen ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        currentPoint = this->getDispCoord(complexZ.real(), complexZ.imag());

        orbitVec.push_back(currentPoint);
        currentstep++;
    }
    return orbitVec;
}

void Mandelbrot::calcMandelbrot()
{
    // This Function calculates the fractal, and then saves it on the
    // .. heap for quick painting.

    QRgb value;

    // Calculate Brot
    int nIterations = 0;

    for (int ptX = 0; ptX < width(); ptX++)
    {
        for (int ptY = 0; ptY < height(); ptY++)
        {
            nIterations = calcIterations(static_cast<double>(ptX),
                                        static_cast<double>(ptY));
            if (nIterations == 0) value = qRgb(0,0,0);
            else value = qRgb(255, 255-nIterations, 255-nIterations);
            this->_brotImage->setPixel(ptX, ptY, value);
        }
    }
}

QSharedPointer<QImage> Mandelbrot::getImage()
{
    return this->_brotImage;
}

void Mandelbrot::recieveJuliaCoord(QPointF clickCoord)
{
    this->_orbit = this->calcOrbit(clickCoord);
    update();
}

void Mandelbrot::paintEvent(QPaintEvent*)
{
    QPainter oPainter(this);
    oPainter.setRenderHint(QPainter::Antialiasing);

    // Paint The Image
    oPainter.drawImage(0, 0, *(this->_brotImage));
    int orbitSize = this->_orbit.size();
    if ( orbitSize > 0 )
    {
        // For each QPoint in the orbit vector, we draw a filled circle.
        // We then loop through again, and draw a line connecting the
        // .. adjacent points. Likely need to swap the order of these two
        // .. operations to have the circles over the lines.
        QPen oPen;
        QBrush oBrush;
        int circleRadius = 2;
        QColor lineColor = Qt::cyan;
        QColor circleColor = Qt::yellow;

        for (int i = 0; i < (orbitSize-1); i++)
        {
            lineColor.setAlpha(255-i*2);
            oPen.setColor(lineColor);
            oPen.setWidth(1);
            oPainter.setPen(oPen);
            oPainter.drawLine(this->_orbit[i], this->_orbit[i+1]);
        }

        for (int i = 0; i < (orbitSize); i++)
        {
            circleColor.setAlpha(255-i*2);
            oPen.setColor(circleColor);
            oBrush.setColor(circleColor);
            if (i == 0)
            {
                oPen.setColor(Qt::magenta);
                oBrush.setColor(Qt::magenta);
            }
            oBrush.setStyle(Qt::SolidPattern);
            oPainter.setPen(oPen);
            oPainter.setBrush(oBrush);
            oPainter.drawEllipse(this->_orbit[i], circleRadius, circleRadius);
        }



    }
}

void Mandelbrot::mousePressEvent(QMouseEvent *event) {
    /* Goal is to get the coordinate of the
     * mouse event and display on the screen */

    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);

    this->_orbit = this->calcOrbit(mathCoord);
    update();
    emit sendMouseCoord(mathCoord);
}

void Mandelbrot::mouseMoveEvent(QMouseEvent *event)
{
    /* Mouse move events will occur only when a mouse button
     * is pressed down, unless mouse tracking has been
     * enabled with QWidget::setMouseTracking().
    */

    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);


    this->_orbit = this->calcOrbit(mathCoord);
    update();
    // Julia Calc is too slow to have it update as I move the mouse
    //    emit sendMouseCoord(mathCoord);
}

void Mandelbrot::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    QPointF mathCoord = this->getMathCoord(xpos, ypos);
    emit sendMouseCoord(mathCoord);

}


