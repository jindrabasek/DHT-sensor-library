/* DHT library

 MIT license
 written by Adafruit Industries
 */
#ifndef DHT_H
#define DHT_H

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <Task.h>

#define ENABLE_DHT_22_ONLY

// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

enum DhtReadState {
    DHT_GOOD, DHT_ERROR
};

class DHT {
private:
    uint8_t data[5];
    uint8_t _pin;
#ifndef ENABLE_DHT_22_ONLY
    uint8_t _type;
#endif

#ifdef __AVR
    // Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
    // for the digital pin connected to the DHT.  Other platforms will use digitalRead.
    uint8_t _bit, _port;
#endif

public:
    DHT(uint8_t pin, uint8_t type = DHT22);

    DhtReadState read();
    float getTemperature();
    float getHumidity();
    static float computeHeatIndex(float temperature, float percentHumidity);
    static float computeAbsoluteHumidity(float temperature, float percentHumidity);

private:
    uint32_t expectPulse(bool level);

};

class InterruptLock {
public:
    InterruptLock() {
        noInterrupts();
    }
    ~InterruptLock() {
        interrupts();
    }
};

#endif
