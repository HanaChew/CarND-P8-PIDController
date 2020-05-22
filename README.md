# CarND-Controls-PID
---
## Goal of project
The ultimate goal of this project is for the car to drive (successfully) around the test track through the implementation of one or more PID controllers.

## Implementation & Theory
From the data, the CTE or cross-track error is given, which is the distance of the car from where it's supposed to be - ideally the center of the lane, but can also differ based on trajectory calculation. This cross track error is very useful in calculating just how much the car would need to steer in order to go back to its' ideal position.

## PID Controller
The PID-Controller is made up of 3 components:
* P - Proportional
* I - Integratal
* D - Derivative

### The Proportional component
This portion is directly proportional to the error measured, and quantified by the proportional gain Kp. This is analogous to the principal of a spring constant, whereby the controller reacts directly to the error given. The term (Kp * pError) should be the largest component in the PID Controller, but is by itself normally insufficient to control the car in this instance.
Increasing Kp tends to increase the response of the system, however increasing it too much leads to an overshoot and possible instability for e.g.; a higher value will mean larger steering angles used to correct the to distance away from the centerline.

### The Integral component
As its name suggests, the integral component acts as an integrator, taking into account past values of the instantaneous error and multiplying it by a certain gain constant Ki. It is thus normally the smallest component in a PID controller, but also the part that slows the response down somewhat - or rather, delays it, as the error accumulates over time.

### The Derivative component
This part takes into account the derivative, or the rate of change of the error over time, in order to predict future errors. The goal of this part of the controller is to eventually reduce the error to zero, and this provides resistance to the entire system.
A very large derivative gain overdamps the system and slows down the convergence.

### Combined into the PID controller
Together, the three parts add up to give information on a system based on past, present and future (predicted) states, and allow the correction of this system. The values need to be tuned in order to get a system that works, as values that are too high or too low would lead to instability and cause the car to veer left and right with increasing amplitude, and eventually drive off the track.

## Implementation
1. The gains for the PID control of the steering were implemented on to the cross-track error (CTE), which is a measure of how far away the car is from the intended waypoints. An initial guess was based on the assumption that the largest contribution would be Kp with 1.0, followed by Kd with 0.1, and then Ki with 0.01 - this resulted in unstable oscillations that grew with time, and it was clear that a number of trials needed to be done manually in order to tune the right gains - even if only for a first draft. A good run was finally achieved with  Kp=0.1, Ki=0.001, Kd=1.0, and a constant throttle setting of 30%. The car never left the track, but did get rather close to the end of the lanes while taking some of the sharp corners.

2. Next, a controller was implemented for the throttle in order to manage the speed. The assumption for the throttle control is that if the steering angle is large, then the speed should then be reduced to account for the turns required. The desired speed should therefore a function of the steering angle of the car - which is a combination of the curve of the track, and any error correction from the CTE that might be happening along the way - and this value was chosen to implement the controller on. For this, the D term was more important than the I term for sure, because if a car sees the tight turns, I want it to think, hey! i need to slow down! So the PID controller is then reduced to a PD controller with a setting of Kp=0.1, Kd=0.1, and this worked well. Setting the maximum throttle input to a 100% unfortunately caused the car to drive a little more erratically even though it stayed on the track (proving the choice of gains was a decent one indeed!), and would definitely work better if the tuning gains are implemented via an automated method such as a twiddle algorithm or so. For the best performance, a maximum throttle input of 70% was chosen since it was more stable for the purposes of this assignment. ;)

3. As an initial starting point, manual implementations of tuning the gains as above are fine, especially since the aims of the assignment are met with the car never leaving the track. But in order to fully optimise the gains, a twiddle algorithm should be applied to obtain better values, and more efficient driving with higher speeds around the track, which would be the next step to implement, and the above results give a good starting point for the implementation of that.

---
## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
