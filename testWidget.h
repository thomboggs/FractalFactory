#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    TestWidget(QWidget *parent);
    ~TestWidget();
    void animate();

public slots:
    void animate_slot();

private:
    void paintEvent(QPaintEvent*) override;

};


#endif // TESTWIDGET_H
