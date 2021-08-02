#ifndef JULIALABEL_H
#define JULIALABEL_H

#include <QLabel>

class JuliaLabel : public QLabel
{
    Q_OBJECT
public:
    JuliaLabel(QWidget *parent = nullptr);
    void calcImage(QPointF zPoint);
    int calcJulia(QPointF zPoint, QPoint cPoint);
    QImage* getImage();
    std::vector<double> getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);
    void recieveBrotCoord(QPointF);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;

signals:
    void sendMouseCoord(QPointF);

private:
    QImage* juliaImage;
    int maxIterations = 20;
};

#endif // JULIALABEL_H
