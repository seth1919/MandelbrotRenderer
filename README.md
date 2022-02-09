# MandelbrotRenderer

### Comments
I made this project before I used GitHub, without the goal in mind of sharing the source code with anybody. Thus, documentation is lacking.

### Description 
I used this project to learn the basics of the windows API in c++ for creating windows applications. 

This program will render the mandelbrot set, which is a shape with a finite volume but infinite perimeter. The mandelbrot set contains every complex 
number c for which the function fc(z) = z^2 + c does not diverge to infinity when iterated from z = 0. The shape
is created by coloring in every point on the complex plane which obeys this rule. Provided that the program is approximating the mandelbrot set with 
enough accuracy, no matter how far you zoom in on the border, you will always see infinitely more detail. In other words, the boundary of the mandelbrot set is a fractal.

### How to Use
This project was made in Visual Studio. I recomend cloning and compiling the repository from there. Use the arrow keys to pan around and the 'I' and 'O' keys to zoom in and out.
If you are having performance issues you can resize the window to a smaller resolution or change the detail variable in MandelbrotMain.cpp on line 19. Increasing the detail will
increase the accuracy of the result, but increase the render time.

### Resources
I found this youtube tutorial helpful for learning the windows API. https://www.youtube.com/watch?v=luuyjjOxnUI&list=PL7Ej6SUky135IAAR3PFCFyiVwanauRqj3
