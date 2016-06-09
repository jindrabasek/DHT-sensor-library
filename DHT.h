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
#include <Logger.h>

#define ENABLE_DHT_22_ONLY

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

enum DhtReadState {
    DHT_GOOD, DHT_ERROR_EXPECT_LOW, DHT_ERROR_EXPECT_HIGH, DHT_ERROR_TIMEOUT_PULSE, DHT_ERROR_CHECKSUM
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
