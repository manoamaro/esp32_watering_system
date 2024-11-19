#include "analog_sensor.hpp"

AnalogSensor::AnalogSensor(gpio_num_t sensorPin, uint16_t min, uint16_t wetValue) : sensorPin(sensorPin), min(min), max(wetValue)
{
    pinMode(sensorPin, INPUT);
}

AnalogSensor::~AnalogSensor()
{
}

uint16_t AnalogSensor::read()
{
    sensorValue = analogRead(sensorPin);
    sensorValue = constrain(sensorValue, min, max);
    return sensorValue;
}

uint8_t AnalogSensor::getValuePercentage()
{
    return map(sensorValue, min, max, 0, 100);
}

uint8_t AnalogSensor::getInvertedValuePercentage()
{
    return 100 - getValuePercentage();
}