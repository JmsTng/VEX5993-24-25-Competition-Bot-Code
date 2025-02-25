/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       James Tung, AYJ Robotics                                  */
/*    Created:      10/1/2024, 3:58:37 PM                                     */
/*    Description:  Competition code for AYJ Robotics                         */
/*                  Tank Drive                                                */
/*                  HIGH STAKES                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain; // A global instance of vex::brain used for printing to the V5 brain screen

// Motors
motor Left1 = motor(PORT9, ratio18_1, false);
motor Left2 = motor(PORT10, ratio18_1, false);
motor Left3 = motor(PORT8, ratio18_1, true);
motor Right1 = motor(PORT1, ratio18_1, true);
motor Right2 = motor(PORT2, ratio18_1, true);
motor Right3 = motor(PORT3, ratio18_1, false);

motor Intake1 = motor(PORT11, ratio18_1, false); // upper
motor Intake2 = motor(PORT12, ratio18_1, true); // lower

// Group motors together into groups for easier control
motor_group Left = motor_group(Left1, Left2, Left3);
motor_group Right = motor_group(Right1, Right2, Right3);
motor_group Intake = motor_group(Intake1, Intake2);

// Pneumatic control
digital_out Claw = digital_out(Brain.ThreeWirePort.A);

// Colour
// optical Optical = optical(PORT13);


// Other devices
controller Controller = controller(primary);

// Variables
bool remoteEnable = true; // Competition switch
int deadzone = 5; // For controller

double wheelDiameterIn = 3.25; // in ft
double wheelDiameterMm = 82.55; // in mm
double wheelTravelIn = 10.24; // circumference in inches
double wheelTravelMm = 260; // circumference in mm

/* HELPER FUNCTIONS */
/**
 * Calculates the wheel angle for a specified linear distance (arc length).
 *
 * @param distance A double for distance IN MM.
 * @return The wheel angle to achieve the specified distance.
 */
double calculateAngle(double distance) {
  return distance / (wheelDiameterMm * 3.1415 / 360);
}

/**
 * Takes a linear distance and calculates how much the wheels need to turn in order to get there.
 * 
 * @param distance The linear distance to travel.
 * @param imperial Whether the distance is given in inches or millimeters.
 */
void driveDistance(double distance, bool imperial) {
  double angle;

  if (imperial) { // in inches
    angle = calculateAngle(distance * 25.4);
  } else { // in mm
    angle = calculateAngle(distance);
  }

  Left.spinFor(forward, angle, degrees, false);
  Right.spinFor(forward, angle, degrees);
}

/**
 * Calculates how much the wheels need to move in order to turn the robot a specified amount.
 *
 * @param theta The desired angle to turn.
 */
void turnAngle(double theta) {
  double distance = 360 * theta / 3.1415;
  double wheelAngle = calculateAngle(distance);

  Left.spinFor(forward, wheelAngle, degrees, false);
  Right.spinFor(forward, -wheelAngle, degrees);
}

void driveRoutine() {
  driveDistance(24, true);
  turnAngle(90);
  driveDistance(12, true);
}

/**
 * Starts up intake (forwards - into bot).
 * Bound to L1 bumper button.
 */
void toggleIntake() {
    static bool running = 0;
    running ^= 1; // toggle (0 -> 1, 1 -> 0)

    Intake.spin(forward, 100 * running, percent);
}

/**
 * Toggles state of MoGo claw. Default open.
 * Bound to R1 bumper button.
 */
void toggleClaw() {
    static bool grabbing = 1; // 1 when not grabbing, -1 when grabbing

    Claw.set(grabbing);
    grabbing ^= 1;
}

double powKeepSign(double x, int power) {
    int sign;

    if (x < 0 && power % 2 == 0) sign = -1;
    else                         sign = 1;

    return (pow(x, power) / pow(100, power-1)) * sign;
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    int yLeft = abs(Controller.Axis3.position()) < deadzone ? 0 : Controller.Axis3.position();
    int yRight = abs(Controller.Axis2.position()) < deadzone ? 0 : Controller.Axis2.position();
    

    Left.spin(forward, -yLeft, percent);
    Right.spin(forward, -yRight, percent);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  Controller.ButtonR1.pressed(toggleClaw);
  Controller.ButtonUp.pressed(toggleIntake);

  Controller.ButtonDown.pressed(drive);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
