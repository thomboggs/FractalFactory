#include "mandelbrotlabel.h"
#include <complex>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <QDebug>
#include <vector>
#include <QPixmap>
#include <QImage>
#include <QPainter>

MandelbrotLabel::MandelbrotLabel(QWidget *parent) : QLabel(parent)
{
    this->resize(800, 800);

    // Initialize the image data on the heap
    this->brotImage = new QImage(width(), height(), QImage::Format_RGB32);

    this->calcMandelbrot();
}

QPoint MandelbrotLabel::getDispCoord(double ptX, double ptY) {
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

std::vector<double> MandelbrotLabel::getMathCoord(int ptX, int ptY)
{
    double xMath = 4*((double)ptX / width()) - 2;
    double yMath = 4*((double)ptY / height()) - 2;
    std::vector output = {xMath, yMath};
//    qDebug() << QString::number(output[0]) << QString::number(output[1]);
    return output;
}

int MandelbrotLabel::getColorValue(double ptX, double ptY)
{
    std::complex<double> complexPoint(4*(ptX / width()) - 2,
                                      4*(ptY / height()) - 2);
    std::complex<double> complexZ(0, 0);

    int nIterations = 1;

    while ((abs(complexZ) < 2 ) && ( nIterations <= m_nSensitivity ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        nIterations++;
    }

    if (nIterations < m_nSensitivity)
        return static_cast<int>(( 255 * nIterations ) / m_nSensitivity );
    else
        return 0;
}

std::vector<QPoint> MandelbrotLabel::calcOrbit(double ptX, double ptY, int maxOrbitLen = 100) {
    //  Do the same calculation as get color value
     qDebug() << "New Orbit Calculation: ";
    std::complex<double> complexPoint(ptX, ptY);
    std::complex<double> complexZ(0, 0);

    qDebug() << "Input Coord: " << QString::number(ptX) << QString::number(ptY);
//    qDebug() << "Initial Complex Point: " << QString::number(complexPoint.real()) << QString::number(complexPoint.imag());
    int maxPoints = maxOrbitLen;

    QPoint currentPoint = this->getDispCoord(ptX, ptY);
    std::vector<QPoint> orbitVec = {currentPoint};

    int currentstep = 0;
    while ((abs(complexZ) < 2 ) && ( currentstep <= maxPoints ))
    {
        complexZ = complexZ * complexZ + complexPoint;
        currentPoint = this->getDispCoord(complexZ.real(), complexZ.imag());
//        qDebug() << "Intermediate ComplexZ: " << QString::number(complexZ.real()) << QString::number(complexZ.imag());

        orbitVec.push_back(currentPoint);
        currentstep++;
    }
//    qDebug() << "Total Orbit Steps: " << QString::number(currentstep);
    return orbitVec;
}

void MandelbrotLabel::calcMandelbrot()
{
    // This Function calculates the fractal, and then saves it on the
    // .. heap for quick painting.

    QRgb value;

    // Calculate Brot
    int nColorValue = 0;

    for (int ptX = 0; ptX < width(); ptX++)
    {
        for (int ptY = 0; ptY < height(); ptY++)
        {
            nColorValue = getColorValue(static_cast<double>(ptX),
                                        static_cast<double>(ptY));
            value = qRgb(nColorValue, 0, 0);
            this->brotImage->setPixel(ptX, ptY, value);
        }
    }
}

void MandelbrotLabel::paintEvent(QPaintEvent*)
{
    QPainter oPainter(this);
    oPainter.setRenderHint(QPainter::Antialiasing);

    QPen oPen;

    // Paint The Image
    oPainter.drawImage(0, 0, *(this->brotImage));
//    this->setPixmap(QPixmap::fromImage(*(this->brotImage)));

    if ( this->orbit.size() > 0 )
    {
        // For each QPoint in the orbit vector, we draw a filled circle.
        // We then loop through again, and draw a line connecting the
        // .. adjacent points. Likely need to swap the order of these two
        // .. operations to have the circles over the lines.

        QBrush oBrush;

        for (int i = 0; i < ((int)this->orbit.size()-1); i++)
        {
            // Draw line
            oPen.setColor(Qt::cyan);
            oPainter.setPen(oPen);
            oBrush.setColor(Qt::blue);
            oPainter.setBrush(oBrush);
            oPainter.drawLine(this->orbit[i], this->orbit[i+1]);
        }

        for (auto point : this->orbit)
        {
            oPen.setColor(Qt::yellow);
            oPainter.setPen(oPen);
            oBrush.setColor(Qt::yellow);
            oPainter.setBrush(oBrush);
            oPainter.drawEllipse(point, 5, 5);
        }



    }
}

void MandelbrotLabel::mousePressEvent(QMouseEvent *event) {
    /* Goal is to get the coordinate of the
     * mouse event and display on the screen */

    QPoint clickPos = event->pos();
    const int xpos = clickPos.x();
    const int ypos = clickPos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);
//    this->mCoord.setText(QString('new Coord'))
    qDebug() << xpos << ypos;
    qDebug() << QString::number(mathCoord[0]) << QString::number(mathCoord[1]);

    this->orbit = this->calcOrbit(mathCoord[0], mathCoord[1]);
    update();
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}

void MandelbrotLabel::mouseMoveEvent(QMouseEvent *event)
{
    /* Mouse move events will occur only when a mouse button
     * is pressed down, unless mouse tracking has been
     * enabled with QWidget::setMouseTracking().
    */
//    if (this->mouseClicked)
//    {

//    }
    QPoint movePos = event->pos();
    const int xpos = movePos.x();
    const int ypos = movePos.y();

    std::vector<double> mathCoord = this->getMathCoord(xpos, ypos);

//    qDebug() << QString::number(mathCoord[0]) << QString::number(mathCoord[1]);

    this->orbit = this->calcOrbit(mathCoord[0], mathCoord[1]);
    update();
    emit sendMouseCoord(QPointF(mathCoord[0], mathCoord[1]));
}

QImage* MandelbrotLabel::getImage()
{
    return brotImage;
}

void MandelbrotLabel::recieveJuliaCoord(QPointF clickCoord)
{
    this->orbit = this->calcOrbit(clickCoord.x(), clickCoord.y());
    update();
}

