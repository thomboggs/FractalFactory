#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <QLabel>

class Mandelbrot : public QLabel
{
    Q_OBJECT
public:
    explicit Mandelbrot(QWidget *parent = nullptr);
    int calcIterations(double ptX, double ptY);
    QPointF getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);
    std::vector<QPoint> calcOrbit(QPointF);
    void calcMandelbrot();
    QImage* getImage();

public slots:
    void recieveJuliaCoord(QPointF);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void sendMouseCoord(QPointF);

private:
    int _minIterations = 1;
    int _maxIterations = 100;
    std::vector<QPoint> _orbit;
    QImage* _brotImage;
    int _maxOrbitLen = 100;

};

#endif // MANDELBROT_H
