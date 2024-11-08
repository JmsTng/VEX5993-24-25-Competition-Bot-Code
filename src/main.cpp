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
#include <cmath>
#include "vex.h"

using namespace vex;

// ******************************* DEFINITIONS ******************************* //
brain Brain; // A global instance of vex::brain used for printing to the V5 brain screen

// Motors
motor Left1 = motor(PORT17, ratio18_1, false);
motor Left2 = motor(PORT16, ratio18_1, false);
motor Right1 = motor(PORT19, ratio18_1, true);
motor Right2 = motor(PORT20, ratio18_1, true);

motor Intake1 = motor(PORT11, ratio18_1, false); // upper
motor Intake2 = motor(PORT12, ratio18_1, true); // lower

motor Claw = motor(PORT15, ratio18_1, false);

// Group motors together into groups for easier control
motor_group Left = motor_group(Left1, Left2);
motor_group Right = motor_group(Left1, Left2);

motor_group Intake = motor_group(Intake1, Intake2);



// Other devices
bool remoteEnable = true;
competition Competition = competition();
controller Controller = controller(primary);
int deadzone = 5;

// Tests
void testSpinAll() {
    Left1.spin(forward, 100, percent);
    Left2.spin(forward, 100, percent);
    Right1.spin(forward, 100, percent);
    Right2.spin(forward, 100, percent);

    Brain.Screen.print("Spin drive");
    Brain.Screen.newLine();

    Claw.spinTo(90, degrees);
    Claw.spinTo(0, degrees);
}


// Common Instructions
/**
 * Stops all motors.
 */
void driveStop() {
    Left.stop();
    Right.stop();
}

void driveLeft(double speed) {
    Left.spin(forward, speed, percent);
}

void driveRight(double speed) {
    Right.spin(forward, speed, percent);
}

void toggleIntake() {
    static short running = 0;
    running ^= 1;

    Intake.spin(forward, 100 * running, percent);
}

void toggleClaw() {
    static bool grabbing = false;

    if (grabbing) {
        // let go
        Claw.spinTo(90, degrees);
        grabbing = !grabbing;
    } else {
        Claw.spinTo(-90, degrees)
        grabbing = !grabbing;
    }
}


// Main
int main() {
    Controller.ButtonL1.pressed(toggleIntake);
    Controller.ButtonR1.pressed(toggleClaw);
    
    while (1) {
        int x = abs(Controller.Axis1.position()) < deadzone ? 0 : Controller.Axis1.position();
        int y = abs(Controller.Axis2.position()) < deadzone ? 0 : Controller.Axis2.position();

        driveLeft(y + x);
        driveRight(y - x);
    }
}
