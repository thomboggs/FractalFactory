#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <QWidget>

class Mandelbrot : public QWidget
{
    Q_OBJECT

public:
    Mandelbrot(QWidget *parent);
    int getColorValue(double ptX, double ptY);
    std::vector<double> getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);
    std::vector<QPoint> calcOrbit(double ptX, double ptY, int maxOrbitLen);
    void calcMandelbrot();
    QImage* getImage();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_nBrightness = 1;
    int m_nSensitivity = 60;
    QString clickCoord = "0.0, 0.0";
    std::vector<QPoint> orbit;
    QImage* brotImage;

};

#endif // MANDELBROT_H
