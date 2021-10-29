# Mandelbrot
Mandelbrot/Julia Set Visualizer

GIF of the FractalFactory in action:
![FractalFactory](https://user-images.githubusercontent.com/10662781/139357005-e106f5d5-f265-4c6c-836a-7efd3fec1e0e.gif)


This program visualizes Mandelbrot Fractal Orbits and Julia Set Fractals for the coordinates clicked by the user. In Addition, the user can save the Julia Set Fractal to their local disk.

- "What is a Mandelbrot set and Julia set?"
  - From Wikipedia: The Mandelbrot set is the set of complex numbers, c, for which the function f_{c}(z)=z^{2}+c does not diverge when iterated from z=0, i.e., for which the sequence f_{c}(0), f_{c}(f_{c}(0)), etc... remains bounded in absolute value. https://en.wikipedia.org/wiki/Mandelbrot_set

  - The Julia Set is the same function, but z doesn't necessarily start at 0. 

- "What is an Orbit?"
  - The orbit refers the to sequence of coordinates that are calculated as the function f_{c}(z)=z^{2}+c is applied to itself over and over again. 

For more info, see the following interesting youtube video: https://www.youtube.com/watch?v=FFftmWSzgmk

## Future Work:
- Add more threads to the Julia Calculation to speed it up (I have already added the MUtex functionality to make it thread safe)
  - Thread architecture may have to be changed.   
- Fix Menu Button
- Update window coordinate system to allow for window resizing.
- Add user coloring options
- For Julia Calc, have a multistep render process for quick view and also detailed view (If concurrency not fast enough)
- Add zoom feature or user selectable scale and center

## Instructions to Build the Project
### Using Qt Creator and QMake
1. Download Qt open-source (all libraries and Qt Creator) from https://www.qt.io/download-open-source (Scroll to the bottom to "Download the Qt Online Installer". May need to create an Account)
2. Download the Mandelbrot project to your computer. 
3. Open the \*.pro file in Qt Creator
4. Build the project

## Overview of your code structure
The project is divided into a few classes, MainWindow, Mandelbrot, Julia, JuliaWorker.

*main.cpp*
- Main is used only to start the application and initialize the window from "MainWindow".

*MainWindow*
- In the constructor for "MainWindow", the classes objects for Mandelbrot and Julia are created and initialized. 
- The menu items and layout system are created and initialized. 
- In the menu, there are popup dialog boxes to provide info, you can save the julia set image to disk, and you can exit the application.

*Mandelbrot*
- This class contains the functions to generate the mandelbrot fractal and to generate the orbits. 
- The class inherits from a QLabel because the QLabel can be used as a frame to display images.
- The calcMandelbrot function is run when the object is initialized to calculate the background for the orbits. 
  - Within this function, calcIterations is run on the coordinate corresponding to each pixel to determine the number of iterations of the fractal formula that is required to prove that the coordinate magnitude will converge to infinity.
- calcOrbit generates a vector of QPoints showing each step of the recursive fractal equation. 
- The paintevent function redraws the Mandelbrot fractal to wipe away previous orbit, then redraws the new orbit on top.
- The mouse event functions catch mouth click/move events and sends signals to the appropriate slots to call the draw functions.
- Switching between pixel coordinates and complex coordinates are handled by getMathCoord and getDispCoord.
- A getter function for the image is included for use in MainWindow

*Julia*
- This class draws the Julia Set Fractal, and handles mouse Events, but the calculation of the fractal is offloaded to the JuliaWorker Class.
- This class also inherits from a QLabel because the QLabel can be used as a frame to display images.
- When initialized, this class allocates the memory for the julia set fractal. It then calculates the fractal at an intial condition of (0,0).
- The *calcJulia* function creates a thread and an instance of the worker function, and then dispatch the worker to calculate the fractal. THe thread signals when complete and deletes itself. Once complete, the *recieveWorkerData* function is run and the display is updated. 
- Switching between pixel coordinates and complex coordinates are handled by getMathCoord and getDispCoord.
- The paintEvent draws the fractal in the _juliaImage imagebuffer.
- A getter function for the image is included for use in MainWindow

*JuliaWorker*
- This class calculates the Julia Set Fractal based on a given coordinate, and exists on it's own thread so that the orbits are draws very smoothly. 
- When a new coordinate is generated from a mouse click or release event, the *process* function is called. This function generates the new fractal image, while *calcJulia* performs the recursive calculation. This function works directly on the image buffer owned by Class Julia. Locking is implemented to prevent accessing the image while being calculated. The mutex was created by Class Julia, but passed by reference into the worker class. 
- In the destructor, *finished* is emitted. This starts a chain where the worker instance and the thread are both properly deleted. This destructor is only called when the application closes.   

## Rubric Satisfaction
- Readme Section
  - Readme is included with Build instructions, project description, and rubric points (*this*).
- Compiling and Testing
  - The code builds using qmake and Qt Creator
- Loops, Functions, I/O
  - Code is organized into functions with multiple control structures. (All Over)
  - The project reads or writes to an external file. (mainwindow.cpp, line: 115)
  - The project accepts input from user as part of its operation. (mouse events across both mandelbrot and julia)
- Object Oriented Programming
  - The Project **IS** split into classes.
  - Class constructors utilize member initialization lists (julia.cpp, line 18) (juliaworker.cpp, line 7)
  - Classes encapsulate behavior
  - Classes follow an appropriate inheritance hierarchy and Derived class functions override virtual base class functions. Appropriate pure virtual functions are overridden according to Qt best practices. Inheriting QLabel for Julia and Mandelbrot (julia.h line 10 and mandelbrot.h line 6)
- Memory Management
  - RAII is used. Beyond the shared pointer to julia image, the structure of the Qt app is RAII. Parent child structure during initialization ensures appropriate deletion.   (julia.cpp line 22)
  - The project uses smart pointers instead of raw pointers. Qt Widgets must be "raw pointers", but they exist in a parent-child relationship that keeps them scoped. Other pointers are shared. (julia.h, line 36)
- Concurrency
  - The project uses multithreading. (julia.cpp, line 60)
  - A mutex or lock is used in the project. (juliaworker.cpp line 19)


## Resources Used in helpiong me with this Project:
- numberphile Youtube video of Orbits (todo: add link)
- Mathologer Youtube video of bhuddha-brot (todo: add link)
- https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
- 
