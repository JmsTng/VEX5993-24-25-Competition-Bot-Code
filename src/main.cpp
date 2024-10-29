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

// A global instance of vex::brain used for printing to the V5 brain screen
brain Brain;

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
    Intake.stop();
    Claw.stop();
}

/**
 * Spin wheels forward for a specified angle, at a specified speed.
 * 
 * @param angle double - angle in degrees to spin wheels
 * @param speed double - percentage of full speed to drive at
 */
void driveForward(double angle, double speed) {
    Left.setVelocity(speed, percent);
    Right.setVelocity(speed, percent);
    Left.spinTo(angle, degrees);
    Right.spinTo(angle, degrees);
}

/**
 * Spin wheels forward for a specified length of time, at a specified speed.
 * 
 * @param time double - time in seconds to drive
 * @param speed double - percentage of full speed to drive at
 */
void driveForward(double time, double speed) {

    Left.setVelocity(speed, percent);
    Right.setVelocity(speed, percent);
    Left.spinFor(time, seconds);
    Right.spinFor(time, seconds);
}


// Main
int main() {
    testSpinAll();
    
}
