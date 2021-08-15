#ifndef JULIAWORKER_H
#define JULIAWORKER_H

#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>

class JuliaWorker : public QObject
{
    Q_OBJECT
public:
    explicit JuliaWorker(QSharedPointer<QImage> juliaImage, QMutex& mtx);
    ~JuliaWorker();
    void process(QPointF zPoint);
    int calcJulia(QPointF zPoint, QPoint cPoint);

signals:
    void calcComplete();
    void finished();

private:
    QSharedPointer<QImage> _juliaImage;
    QMutex& _mutex;
    int _brightness = 1;
    int _maxIterations = 100;
    int _xsize = 800;
    int _ysize = 800;
};

#endif // JULIAWORKER_H
