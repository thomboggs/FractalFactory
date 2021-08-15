#ifndef JULIA_H
#define JULIA_H

#include <QLabel>
#include <QSharedPointer>
#include <QMutex>
#include <QMutexLocker>


class Julia : public QLabel
{
    Q_OBJECT
public:
    Julia(QWidget *parent = nullptr);

    QSharedPointer<QImage> getImage();
    QPointF getMathCoord(int ptX, int ptY);
    QPoint getDispCoord(double ptX, double ptY);
    void calcJulia(QPointF);

public slots:
    void recieveBrotCoord(QPointF);
    void recieveWorkerData();

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

signals:
    void sendMouseCoord(QPointF);
    void calcJuliaSet(QPointF);

private:
    QSharedPointer<QImage> _juliaImage;
    QMutex _mutex;
};

#endif // JULIA_H
