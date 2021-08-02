#ifndef MANDELBROTLABEL_H
#define MANDELBROTLABEL_H

#include <QLabel>

class MandelbrotLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MandelbrotLabel(QWidget *parent = nullptr);
    int getColorValue(double ptX, double ptY);
    std::vector<double> getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);
    std::vector<QPoint> calcOrbit(double ptX, double ptY, int maxOrbitLen);
    void calcMandelbrot();
    QImage* getImage();
    void recieveJuliaCoord(QPointF);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void sendMouseCoord(QPointF);

private:
    int m_nBrightness = 1;
    int m_nSensitivity = 100;
    QString clickCoord = "0.0, 0.0";
    std::vector<QPoint> orbit;
    QImage* brotImage;

};

#endif // MANDELBROTLABEL_H
