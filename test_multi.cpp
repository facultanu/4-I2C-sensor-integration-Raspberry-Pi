/* ******************************* XENI ROBOTICS *******************************
 * A test program to test the functionality of the vl53l1x library
 * Developed by Eric Slaghuis, Xeni Robotics, 18 August 2021
 *
 * g++ test_multi.cpp vl53l1x_multi.cpp -o test_multi
 *
 * ********************************** X *** X ********************************** */

/* *******************************************************************
 * A test program to test 4 VL53L1X sensors on independent I2C buses
 * *******************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h> 
#include "vl53l1x_multi.h"

using namespace std;

int main() {
    cout << "Setting up independent I2C buses..." << endl;

    // Instantiate 4 sensors on their dedicated I2C buses (1, 3, 5, 6)
    Vl53l1x sensor1("/dev/i2c-1");
    Vl53l1x sensor2("/dev/i2c-3");
    Vl53l1x sensor3("/dev/i2c-5");
    Vl53l1x sensor4("/dev/i2c-6");

    // Put them in an array so we can loop through them easily
    Vl53l1x* sensors[] = {&sensor1, &sensor2, &sensor3, &sensor4};

    // Initialize all sensors
    for (int i = 0; i < 4; i++) {
        cout << "Initializing sensor " << i+1 << "..." << endl;
        sensors[i]->setTimeout(500);
        
        if (!sensors[i]->init()) {
            cerr << "❌ Failed to initialize sensor " << i+1 << ". Check its wiring!" << endl;
            return -1;
        }

        // Configure ranging parameters (Long distance, 50ms budget)
        sensors[i]->setDistanceMode(Vl53l1x::Long);
        sensors[i]->setMeasurementTimingBudget(50000);
        sensors[i]->startContinuous(50);
    }

    cout << "\n✅ All sensors initialized successfully! Starting reads...\n" << endl;

    // Read loop (Runs for 100 cycles as a test)
    for(auto cycle = 0; cycle < 100; cycle++) {
        cout << "--- Cycle " << cycle + 1 << " ---" << endl;
        for (int i = 0; i < 4; i++) {
            int altitude = sensors[i]->read_range(); // Change to read_range() or whatever your specific library uses if it throws an error here
            
            if(sensors[i]->timeoutOccurred()) {
                cout << "Sensor " << i+1 << ": TIMEOUT" << endl;
            } else {
                cout << "Sensor " << i+1 << ": " << altitude << " mm" << endl;
            }
        }
        usleep(50000); // Wait 50ms before asking for the next reading
    }

    // Safely shut down the lasers before exiting
    cout << "Cleaning up..." << endl;
    for(int i = 0; i < 4; i++) {
        sensors[i]->stopContinuous();
    }
    
    return 0;
}