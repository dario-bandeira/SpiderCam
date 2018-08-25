# SpiderCam

![spidercam demonstration](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_demonstration.gif)

This project is a prototype of a camera like this one for Arduino.
It's still simple and boring, just make an eternal movement in X axis like a loop, but in the future I'm gonna build a version controled by joystick.

To build your own, you have to consider 3 things:

## 1. Box measures
![box measures](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_measures.jpg)

You have to convert the measures to "motor steps", that is, how many steps your motor have to give to roll (for exemple) 40cm of wire. In my case it is 4920, that means my box has 4920 steps high.

## 2. Box Axes
![box axes](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_dimensions.jpg)

You have to define a Zero point and start the measures from there (in steps).

## 3. The ball position
**Important**: the ball position is where the wires meet, not the center of the ball!


# The calculations
![box triangles](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_triangles.jpg)
