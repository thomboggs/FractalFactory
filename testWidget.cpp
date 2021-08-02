#include "testWidget.h"
#include <QPainter>
#include <QLabel>
#include <QPixmap>


TestWidget::TestWidget(QWidget* parent) : QWidget(parent)
{
//    setFixedSize(400, 400);
    QLabel *dispLabel = new QLabel(this);
    QPixmap testMap = QPixmap(100, 100);
    testMap.fill(Qt::blue);
    dispLabel->setPixmap(testMap);
}

TestWidget::~TestWidget()
{

}

void TestWidget::animate()
{
    update();
}

void TestWidget::animate_slot()
{
    update();
}

void TestWidget::paintEvent(QPaintEvent*)
{
    QPainter painter;
    QPen pen;
    QBrush brush;

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    pen.setColor(Qt::yellow);
    painter.setBrush(brush);
    painter.drawEllipse(QPoint(0,0), 20, 20);

}
