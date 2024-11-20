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
#include <algorithm>
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
// The range (0 to 100) through which an input can be varied without initiating an observable response 
const short deadzone = 5;
// Intake speed is global in order to control slow mode based on it
signed short intakeSpeed=0;
// A percentage (0 to 100) representing the magnitude of the fastest speed. 
const short SLOW_MODE_SPEED=50;

//Enumerators for functions; makes code more readable and modular
enum Claw{
    GRABBING=-1,
    NOT_GRABBING=1
};

enum Intake{
    NOT_RUNNING=0,
    RUNNING=1
};

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
    static short running = NOT_RUNNING;
    running ^= RUNNING; // toggle (0 -> 1, 1 -> 0)

    Intake.spin(forward, 100 * running, percent);
}

/**
 * Controls intake based on vertical position of joystick
 * Joystick up - into bot
 * Joystick down - out of bot
 * Will stop intake when joystick is in deadzone
 * Bound to left controller joystick
 */
void joystickIntake(){
    intakeSpeed=abs(Controller.Axis4.position()) < deadzone ? 0 : Controller.Axis4.position();
    Intake.spin(forward, intakeSpeed, percent);
}

/**
 * Toggles state of MoGo claw. Default open.
 * Bound to R1 bumper button.
 */
void toggleClaw() {
    static short grabbing = GRABBING; // 1 when not grabbing, -1 when grabbing

    Claw.spinTo(120 * grabbing, degrees);
    grabbing *= NOT_GRABBING;
}

/**
 * Helper function to cap speed.
 * @param speed A percentage (-100 to 100) representing how fast to go.
 * @param cap A percentage (0 to 100) representing the magnitude of the fastest speed.
 */
int capSpeed(int speed, int cap) {
    if(abs(speed)>cap){
        if(speed>0){
            return cap;
        }
        return -cap;
    }
    return speed;
}

// Main
int main() {
    Controller.ButtonL1.pressed(toggleIntake);
    Controller.ButtonR1.pressed(toggleClaw);
    
    while (1) {
        /*
        Run drivetrain functions
        */
        int x = abs(Controller.Axis1.position()) < deadzone ? 0 : Controller.Axis1.position();
        int y = abs(Controller.Axis2.position()) < deadzone ? 0 : Controller.Axis2.position();

        
        //Caps motor speed when the intake is active to allow finer turning control.
        //Hopefully (?) helps when intaking rings
        if(intakeSpeed!=0){
            x=capSpeed(x,SLOW_MODE_SPEED);
            y=capSpeed(y,SLOW_MODE_SPEED);
        }

        driveLeft(y - x);
        driveRight(y + x);
        /*
        Run intake functions
        */
        joystickIntake();

        wait(5, msec);
    }
}
