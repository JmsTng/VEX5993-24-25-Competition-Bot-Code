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
motor Left1 = motor(PORT17, ratio18_1, false);
motor Left2 = motor(PORT16, ratio18_1, false);
motor Right1 = motor(PORT19, ratio18_1, true);
motor Right2 = motor(PORT20, ratio18_1, true);

motor Intake1 = motor(PORT11, ratio18_1, false); // upper
motor Intake2 = motor(PORT12, ratio18_1, true); // lower

// Group motors together into groups for easier control
motor_group Left = motor_group(Left1, Left2);
motor_group Right = motor_group(Right1, Right2);
motor_group Intake = motor_group(Intake1, Intake2);

// Pneumatic control
digital_out Claw = digital_out(Brain.ThreeWirePort.A);

// Colour
optical Optical = optical(PORT13);


// Other devices
bool remoteEnable = true;
controller Controller = controller(primary);
int deadzone = 5;
const double wheelCircumference = 319.024;


/* HELPER FUNCTIONS */
/**
 * Spins left-side motors.
 *
 * @param speed A percentage (-100 to 100) representing how fast to go.
 */
void driveLeft(double speed) {
  Left.spin(forward, speed, percent);
}

/**
 * Spins right-side motors.
 *
 * @param speed A percentage (-100 to 100) representing how fast to go.
 */
void driveRight(double speed) {
  Right.spin(forward, speed, percent);
}

/**
 * Spins left-side motors to travel a certain distance.
 *
 * @param distance A double representing travel distance in millimeters.
 */
void driveLeftDist(double distance) {
  Left.spinFor(forward, distance / wheelCircumference * 360, degrees);
}


/**
 * Spins right-side motors to travel a certain distance.
 *
 * @param distance A double representing travel distance in millimeters.
 */
void driveRightDist(double distance) {
  Right.spinFor(forward, distance / wheelCircumference * 360, degrees);
}

/**
 * Calculate the 
 */
void turn(double angle) {
    double distancePerDegree = 17*25.4*3.14/360; // Bot circumference in mm

    driveLeftDist(distancePerDegree*angle);
    driveRightDist(-distancePerDegree*angle);
}


/**
 * Starts up intake (forwards - into bot).
 * Bound to L1 bumper button.
 */
void toggleIntake() {
  static short running = 0;
  running ^= 1; // toggle (0 -> 1, 1 -> 0)

  Intake.spin(forward, 100 * running, percent);
}

/**
 * Toggles state of MoGo claw. Default open.
 * Bound to R1 bumper button.
 */
void toggleClaw() {
  static short grabbing = 1; // 1 when not grabbing, -1 when grabbing

  Claw.set(grabbing);
  grabbing ^= 1;
}

/**
 * Maps speed to a quadratic function.
 * Center goes slow, outside is normal.
 *
 * @param speed An integer speed value.
 */
double quadratic(int speed) {
  return speed * abs(speed) / 100;
}

void reportTemps() {
  Brain.Screen.clearScreen();
  Brain.Screen.print("Left1: %f C", Left1.temperature());
  Brain.Screen.newLine();
  Brain.Screen.print("Left2: %f C", Left2.temperature());
  Brain.Screen.newLine();
  Brain.Screen.print("Right1: %f C", Right1.temperature());
  Brain.Screen.newLine();
  Brain.Screen.print("Right2: %f C", Right2.temperature());
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

  Claw.set(0);
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
  // Approach mogo
  driveLeftDist(200);
  driveRightDist(200);

  // Turn around
  turn(230);

  // Attempt mogo grab
  Claw.set(1);

  // Retreat
  turn(-90);
  driveLeftDist(200);
  driveRightDist(200);
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

  int x = abs(Controller.Axis1.position()) < deadzone ? 0 : Controller.Axis1.position();
  int y = abs(Controller.Axis2.position()) < deadzone ? 0 : Controller.Axis2.position();
  int intake = Controller.Axis3.position();

  if (intake > deadzone) {
    Intake.spin(forward, 100, percent);
  } else if (intake < -deadzone) {
    Intake.spin(forward, -100, percent);
  } else {
    Intake.stop();
  }

  driveLeft(quadratic(y - x));
  driveRight(quadratic(y + x));

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
  Controller.ButtonL1.pressed(reportTemps);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
