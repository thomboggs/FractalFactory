#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    void exit();
    void about();
    void tips();

signals:

protected:

private:

};
#endif // MAINWINDOW_H
