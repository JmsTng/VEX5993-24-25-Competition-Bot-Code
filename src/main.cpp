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
motor_group Right = motor_group(Right1, Right2);

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
 * Stops all drivetrain motors.
 */
void driveStop() {
    Left.stop();
    Right.stop();
}

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

    Claw.spinTo(120 * grabbing, degrees);
    grabbing *= -1;
}


// Main
int main() {
    Controller.ButtonL1.pressed(toggleIntake);
    Controller.ButtonR1.pressed(toggleClaw);
    
    while (1) {
        int x = abs(Controller.Axis1.position()) < deadzone ? 0 : Controller.Axis1.position();
        int y = abs(Controller.Axis2.position()) < deadzone ? 0 : Controller.Axis2.position();

        driveLeft(y - x);
        driveRight(y + x);

        wait(5, msec);
    }
}
