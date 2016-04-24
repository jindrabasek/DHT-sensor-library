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
    DHT_GOOD, DHT_NOT_YET_READ, DHT_ERROR
};

enum State {
    READY_FOR_READ_1, HIGH_IMPEDANCE_2, READING_VALUES_3
};

class DHT : public Task {
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
    uint32_t _maxcycles;
    DhtReadState _lastresult;
    State algState;

public:
    DHT(uint8_t pin, uint8_t type = DHT22);
    float readTemperature();
    float computeHeatIndex(float temperature, float percentHumidity);
    float readHumidity();
    DhtReadState read();

    virtual void run();

    DhtReadState getLastresult() const {
        return _lastresult;
    }

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
