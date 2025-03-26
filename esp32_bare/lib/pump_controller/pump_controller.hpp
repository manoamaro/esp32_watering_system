#pragma once

#include <Arduino.h>
#include "pump_config.hpp"

class PumpController
{
public:
    uint8_t pumpIdx;
    PumpConfig config = PumpConfig(pumpIdx);
    // max runtime in milliseconds = 10 minutes
    uint32_t runtimeRange[2] = {0, PUMP_MAX_RUNTIME};
    // percentage of humidity
    int humidityRange[2] = {0, 100};
    // temperature in Celsius
    int temperatureRange[2] = {PUMP_MIN_TEMPERATURE, PUMP_MAX_TEMPERATURE};
    // percentage of light intensity
    int lightRange[2] = {0, 100};
    // last run time in milliseconds
    uint32_t lastRunMillis = 0;
    bool isForceRun = false;

    PumpController(uint8_t pumpIdx);
    bool shouldRun(int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity);
    uint32_t calculateRuntime(int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity);
    bool isRunning(uint32_t currentMillis, int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity);
    void forceRun(uint32_t currentMillis);
    void forceStop(uint32_t currentMillis);
    ~PumpController();
};
