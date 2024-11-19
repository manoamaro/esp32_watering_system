#pragma once

#include <Arduino.h>
#include "Preferences.h"

#define PUMP_CONFIG_PREFS_NAMESPACE "pump_config"
#define PUMP_MAX_FREQUENCY 1000 * 60 * 60 * 24 // 24 hours
#define PUMP_MAX_RUNTIME 1000 * 60 * 30      // 30 minutes
#define PUMP_MAX_TEMPERATURE 50
#define PUMP_MIN_TEMPERATURE -20

class PumpConfig
{
public:
    uint8_t pumpIdx;
    bool enabled = true;
    // Weights for each sensor
    int8_t humidityWeight, temperatureWeight, soilMoistureWeight, lightWeight;
    // thresholds for each sensor, if the value is below (or above) the threshold, the pump will not run
    int minLightTreshold, minTemperatureThreshold, maxSoilMoistureTreshold, maxHumidityThreshold;
    // base runtime in milliseconds, this is the runtime when all sensors are at their optimal values
    uint32_t baseRuntimeMillis = 30000;
    // frequency in milliseconds, this is the time between each pump run
    uint32_t frequencyMillis = 60000;
    u_int16_t soilSensorDryValue = 0;
    u_int16_t soilSensorWetValue = 4095;

    PumpConfig(uint8_t pumpIdx);
    void load();
    void save();
    void incHumidityWeight(int8_t by);
    void incTemperatureWeight(int8_t by);
    void incSoilMoistureWeight(int8_t by);
    void incLightWeight(int8_t by);
    void incMinLightTreshold(int by);
    void incMinTemperatureThreshold(int by);
    void incMaxSoilMoistureTreshold(int by);
    void incMaxHumidityThreshold(int by);
    void incBaseRuntimeMillis(int by);
    void incFrequencyMillis(int by);
    ~PumpConfig();
};