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
    explicit JuliaWorker(QWidget *parent = nullptr);
    ~JuliaWorker();
    void process(QPointF zPoint);
    int calcJulia(QPointF zPoint, QPoint cPoint);

signals:
    void sendImage(QImage*);
    void finished();

private:
    QImage* _juliaImage;
    int _brightness = 1;
    int _maxIterations = 100;
    int _xsize = 800;
    int _ysize = 800;
    QWidget* _parent{nullptr};
};

#endif // JULIAWORKER_H
