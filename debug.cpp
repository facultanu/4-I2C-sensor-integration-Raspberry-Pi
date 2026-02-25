#include <iostream>
#include <pigpio.h>
#include <stdlib.h>
#include "vl53l1x_multi.h"

using namespace std;

int main() {
    if (gpioInitialise() < 0) {
        cout << "Pigpio init failed! Run with sudo." << endl;
        return -1;
    }
    
    int pin = 17; // BCM 17 = Physical Pin 11 (Sensor 1)
    
    cout << "1. Pulling XSDN LOW (Forcing Sensor 1 to Sleep)..." << endl;
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, 0);
    gpioDelay(500000); // Wait half a second
    
    cout << "2. Waking Sensor UP (Driving 3.3V HIGH)..." << endl;
    gpioWrite(pin, 1);
    gpioDelay(500000); // Wait half a second for a full, clean boot
    
    cout << "3. Asking Linux if it can see the sensor on the bus:" << endl;
    cout << "----------------------------------------------------" << endl;
    // This executes standard i2cdetect right inside our C++ program
    system("i2cdetect -y 1");
    cout << "----------------------------------------------------" << endl;
    
    cout << "4. Attempting C++ Library Initialization..." << endl;
    Vl53l1x sensor;
    sensor.setTimeout(500);
    
    if(!sensor.init()) {
        cout << "\n❌ FAILED! The C++ library rejected the sensor." << endl;
    } else {
        cout << "\n✅ SUCCESS! Sensor 1 is fully operational." << endl;
    }
    
    gpioTerminate();
    return 0;
}