# Mandelbrot
Mandelbrot/Julia Set Visualizer

[Describe the application]
- Dual Frame app in a single window. 
- Left side shows Mandelbrot Set, Right side show the julia set at 0,0
- When you click on a chart, the orbit for the mandelbrot set at that coordinate is calculated and the julia set for that coefficient is displayed on the right. 

## Instructions to Build the Project
### Using Qt Creator and QMake
1. Download QT open-source from https://www.qt.io/download-open-source (Scroll to the bottom to "Download the Qt Online Installer". May need to create an Account)
2. Download the Mandelbrot project to your computer. 
3. Open the \*.pro file in Qt Creator
4. Build the project

## Overview of your code structure
The project is divided into a few classes, MainWindow, Mandelbrot, Julia, JuliaWorker.

*main.cpp*
Main is used only to start the application and initialize the window from "MainWindow".

*MainWindow*
In the constructor for "MainWindow", the classes objects for Mandelbrot and Julia are created and initialized. The The menu items and layout system are created and initialized. 

*Mandelbrot*


## Rubric Satisfaction




## Future Work:
- Add more threads to the Julia Calculation to speed it up (I have already added the MUtex functionality to make it thread safe)
- Update window coordinate system to allow for window resizing.
- Add user coloring options
- For Julia Calc, have a multistep render process for quick view and also detailed view (If concurrency not fast enough)
- Add zoom feature or user selectable scale and center
- add saving of output images

## Resources Used in helpiong me with this Project:
- numberphile Youtube video of Orbits (todo: add link)
- Mathologer Youtube video of bhuddha-brot (todo: add link)
- Youtube video of programming Mandelbrot in c++ to help me learn the basics of putting something on the screen (todo: add link)
- https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
- 
