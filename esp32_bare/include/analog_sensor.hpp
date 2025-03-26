#pragma once

#define DEF_GETTER_SETTER(Type, VarName)      \
    Type get##VarName() const { return VarName; } \
    void set##VarName(Type value) { VarName = value; }


#include <Arduino.h>

class AnalogSensor
{
    private:
        gpio_num_t sensorPin;
        uint16_t min, max;
        uint16_t sensorValue;
    public:
        AnalogSensor(gpio_num_t sensorPin, uint16_t min, uint16_t max);
        AnalogSensor(gpio_num_t sensorPin) : AnalogSensor(sensorPin, 0, 4095) {};
        ~AnalogSensor();
        uint16_t read();
        uint8_t getValuePercentage();
        uint8_t getInvertedValuePercentage();
        DEF_GETTER_SETTER(uint16_t, min)
        DEF_GETTER_SETTER(uint16_t, max)
};