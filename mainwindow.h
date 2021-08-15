#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "julia.h"
#include <QWidget>
#include <QSharedPointer>
#include <memory>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    void exit();
    void about();
    void tips();
    void saveJulia();

signals:

protected:

private:
    Julia* _julia;

};
#endif // MAINWINDOW_H
