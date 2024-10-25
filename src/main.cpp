/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       James Tung, AYJ Robotics                                  */
/*    Created:      10/1/2024, 3:58:37 PM                                     */
/*    Description:  Competition code for AYJ Robotics                         */
/*                  Tank Drive                                                */
/*                  HIGH STAKES                                               */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
brain Brain;

// define your global instances of motors and other devices here
motor Left1 = motor(PORT1, ratio18_1, false);
motor Left2 = motor(PORT2, ratio18_1, false);
motor Right1 = motor(PORT9, ratio18_1, true);
motor Right2 = motor(PORT10, ratio18_1, true);

motor Claw = motor(PORT11, ratio18_1, false);


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

int main() {
    testSpinAll();
    
}
