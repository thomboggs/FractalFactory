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
- Main is used only to start the application and initialize the window from "MainWindow".

*MainWindow*
- In the constructor for "MainWindow", the classes objects for Mandelbrot and Julia are created and initialized. 
- The menu items and layout system are created and initialized. 

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
- https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
- 
