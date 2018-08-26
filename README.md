# SpiderCam

![spidercam demonstration](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_demonstration.gif)

This project is a prototype of a camera like <a href="https://youtu.be/wor4BFjbIyI?t=1m32s" target="_blank">this one</a> for Arduino.
It's still simple and boring, just make an eternal movement in X axis like a loop, but in the future I'm gonna build a version controled by joystick.

To build your own, you have to consider 3 things:

## 1. Box measures
![box measures](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_measures.jpg)

You have to convert the measures to "motor steps", that is, how many steps your motor have to give to roll (for exemple) 40cm of wire. In my case it is 4920, that means my box has 4920 steps high.

## 2. Box Axes
![box axes](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_dimensions.jpg)

You have to define a Zero point and start the measures from there (in steps).

## 3. The ball position
It's given by the variables x, y and z, which are the values in the 3 dimensions.<br>
**Important**: the ball position is where the wires meet, not the center of the ball!


# The calculations
![box triangles](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_triangles.jpg)

To find out the wire length, you calculate the hypotenuse of two imaginary triangles (t1 and t2 on the figure) using Pythagorean theorem.
```
hypotenuse = sqrt( pow(leg1, 2) + pow(leg2, 2) );
```
Triangle 1:<br>
leg1 = x<br>
leg2 = box width - y


Triangle 2:<br>
leg1 = the hypotenuse of triangle 1<br>
leg2 = z

The hypotenuse of triangle 2 is the wire.
Repeat this for the 4 wires.

# Movement
You take an imaginary point inside the box where you want the ball to goes to. Then you repeat the calculation above considering that new point.
Now you know the actual lengths of the wires and the lenghts they have to have to hold the ball on the desired position, so make the motors do the movement.

For each wire you do `actualLength - newLength`, if you got a positive value you release wire to inside the box, if you got a negative value you pull wire to outside.

**Caution:** be sure the motors will move synchronized to avoid overload on the wires.

###### How to do this?

Lets supose the motors have to give this amount of steps to reach the desired point:

motor 0: 200 steps<br>
motor 1: 100 steps<br>
motor 2: 60 steps<br>
motor 3: 50 steps

To find out the amount of steps each motor have to give **in a single cycle**, divide its number of steps by the lower one.<br>
For exemple:

```
motor 0: 200 steps
motor 3: 50 steps (the lower value of all)

200/50 = 4
```

So the 'motor 0' have to give 4 steps in each cycle.<br>
Repeat this calculation to all motors.

```
Steps in this cycle:

motor 0: 4 steps
motor 1: 2 steps
motor 2: 1,2 steps
motor 3: 1 step
```

Look at the motor 2, that's a decimal value of steps, you cant do this in a stepper motor. So you have to store the leftout to apply on the next cycle to avoid the wire getting loose.

```
After separing the leftout:

motor 0: 4 steps
motor 1: 2 steps
motor 2: 1 step
motor 3: 1 step

leftout 0: 0
leftout 1: 0
leftout 2: 0,2
leftout 3: 0
```

Do the first cycle, apply the previous leftouts on the next cycle and store the leftouts again...

```
Second cycle will be:

motor 0: 4 steps
motor 1: 2 steps
motor 2: 1 step
motor 3: 1 step

leftout 0: 0
leftout 1: 0
leftout 2: 0,4 <--
leftout 3: 0
```

...until this leftout reaches the value of 1 or more. When it happens, you have 1 more step to give to this motor in the next cycle.

This process will take the ball to the desired point.<br>
**Note:** the total number of cycles is the same of the lower value of steps. In this case, 50.

### For next versions
A joystick (or a keyboard) is gonna tell the directions and the speed of the movement.
