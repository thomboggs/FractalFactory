#include "mainwindow.h"
#include "mandelbrot.h"
#include "julia.h"
#include "juliaworker.h"

#include <QLabel>
#include <QImage>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>
#include <QMessageBox>
#include <string>


MainWindow::MainWindow()
{
    setWindowTitle(tr("Mandelbrot Orbits and Julia Sets"));

    // initialize fractal widgets
    Mandelbrot* brot = new Mandelbrot(this);
    QImage* brotImage = brot->getImage();
    Julia* julia = new Julia(this);
    QSharedPointer<QImage> juliaImage = julia->getImage();

    // set initial pixmap to define space
    brot->setPixmap(QPixmap::fromImage(*brotImage));
    julia->setPixmap(QPixmap::fromImage(*juliaImage));

    // Create Layout with labels for each side
    QLabel *mandelbrotLabel = new QLabel(tr("Mandelbrot Set with Orbits"), this);
    mandelbrotLabel->setAlignment(Qt::AlignHCenter);
    QLabel *juliaLabel = new QLabel(tr("Julia Sets"), this);
    juliaLabel->setAlignment(Qt::AlignHCenter);

    // Add Menu bar on top
    QMenuBar* menubar = new QMenuBar(this);
    QAction* quit = new QAction("&Quit", menubar);
    QAction* about = new QAction("&About", menubar);
    QAction* tips = new QAction("&Tips", menubar);
    QMenu* file_menu = menubar->addMenu("Menu");

    // Set File Menu Order (top to bottom)
    file_menu->addAction(about);
    file_menu->addAction(tips);
    file_menu->addAction(quit);

    // Define file menu actions
    connect(quit, &QAction::triggered, this, &MainWindow::exit);
    connect(about, &QAction::triggered, this, &MainWindow::about);
    connect(tips, &QAction::triggered, this, &MainWindow::tips);

    // Place Plots and labels into grid
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(brot, 0, 0);
    layout->addWidget(julia, 0, 1);
    layout->addWidget(mandelbrotLabel, 1, 0);
    layout->addWidget(juliaLabel, 1, 1);
    this->setLayout(layout);

    connect(brot, &Mandelbrot::sendMouseCoord,
            julia, &Julia::recieveBrotCoord);
    connect(julia, &Julia::sendMouseCoord,
            brot, &Mandelbrot::recieveJuliaCoord);
}

void MainWindow::exit()
{
    QCoreApplication::quit();
}

void MainWindow::about()
{
    QMessageBox mBox = QMessageBox();
    const QString about_title = "Info About Mandelbrot/Julia-Set Visualizer";
    const QString about_text =
            QString("Click on either the left or right plot to see the \n")
            + QString("Mandelbrot Orbit and Julia Set generated from that coordinate.")
            + QString("\n\n")
            + QString("For more information, see:\n")
            + QString("https://github.com/thomboggs/Mandelbrot/tree/main#readme");
    mBox.about(this, about_title, about_text);
}

void MainWindow::tips()
{
   QMessageBox mBox = QMessageBox();
   const QString about_title = "Helpful Tips";
   const QString about_text =
           QString("1: You can move the application window, but do not resize the window.\n")
           + QString("2: You can click and drag the mouse to view the orbits change smoothly.\n")
           + QString("3: The Julia Set Fractal is updated when you press the mouse button, and when you release the mouse button.\n")
           + QString("4: Notice the correlation between orbits and the shape of the julia set. ")
           + QString("For instance, the rotation of the arms of the julia set matches the rotation of the orbit.");
   mBox.about(this, about_title, about_text);
}

