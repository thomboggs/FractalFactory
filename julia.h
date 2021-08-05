#ifndef JULIA_H
#define JULIA_H

#include <QLabel>
#include <QSharedPointer>

class Julia : public QLabel
{
    Q_OBJECT
public:
    Julia();
//    void calcImage(QPointF zPoint);
//    int calcJulia(QPointF zPoint, QPoint cPoint);
    void setImage(QImage*);
    QImage* getImage();
    std::vector<double> getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);

public slots:
    void recieveBrotCoord(QPointF);
    void recieveWorkerData(QImage*);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

signals:
    void sendMouseCoord(QPointF);
    void calcJuliaSet(QPointF);

private:
    QImage* juliaImage;
//    QPixmap juliaPixmap;
    int maxIterations = 20;
};

#endif // JULIA_H
