#ifndef JULIAWORKER_H
#define JULIAWORKER_H

#include <QObject>
#include <QPointF>
#include <QImage>
#include <memory>
#include <QPixmap>

class JuliaWorker : public QObject
{
    Q_OBJECT
public:
    explicit JuliaWorker(QObject *parent = nullptr);
    ~JuliaWorker();
    void calcImage(QPointF zPoint);
    int calcJulia(QPointF zPoint, QPoint cPoint);

signals:
    void sendImage(QImage*);
    void finished();

private:
    QImage* juliaImage;
    int maxIterations = 50;
    int xsize = 800;
    int ysize = 800;
};

#endif // JULIAWORKER_H
