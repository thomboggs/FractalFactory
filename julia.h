#ifndef JULIA_H
#define JULIA_H

#include <QWidget>
#include <QPoint>
#include <QPointF>

class Julia : public QWidget
{
    Q_OBJECT
public:
    explicit Julia(QWidget *parent);
    ~Julia();
    void calcImage(QPointF zPoint);
    int calcJulia(QPointF zPoint, QPoint cPoint);
    QImage* getImage();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QImage* juliaImage;
    int maxIterations = 40;
};

#endif // JULIA_H
