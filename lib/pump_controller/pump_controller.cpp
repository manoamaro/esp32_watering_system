#include "pump_controller.hpp"

PumpController::PumpController(uint8_t pumpIdx) : pumpIdx(pumpIdx)
{
}

bool PumpController::shouldRun(int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity)
{
    return config.enabled && (isForceRun ||
                              currentHumidity <= config.maxHumidityThreshold &&
                                  currentTemperature >= config.minTemperatureThreshold &&
                                  currentSoilMoisture <= config.maxSoilMoistureTreshold &&
                                  currentLightIntensity >= config.minLightTreshold);
}

u_int32_t PumpController::calculateRuntime(int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity)
{
    if (!shouldRun(currentHumidity, currentTemperature, currentSoilMoisture, currentLightIntensity))
    {
        return 0;
    }
    if (isForceRun)
    {
        return config.baseRuntimeMillis;
    }

    // Normalize values with weights
    double light_factor = static_cast<double>(currentLightIntensity - lightRange[0]) / (lightRange[1] - lightRange[0]) * config.lightWeight / 100.0;
    double temperature_factor = static_cast<double>(currentTemperature - temperatureRange[0]) / (temperatureRange[1] - temperatureRange[0]) * config.temperatureWeight / 100.0;
    double humidity_factor = static_cast<double>(currentHumidity - humidityRange[0]) / (humidityRange[1] - humidityRange[0]) * config.humidityWeight / 100.0;

    // Weighted run time factor
    double weighted_run_time_factor = (light_factor + temperature_factor + humidity_factor) / (static_cast<double>(config.lightWeight + config.temperatureWeight + config.humidityWeight) / 100.0);

    // Calculate initial run time
    double R_initial = config.baseRuntimeMillis * weighted_run_time_factor;

    // Ensure run time is within limits
    u_int32_t R = max(runtimeRange[0], min(static_cast<u_int32_t>(R_initial), runtimeRange[1]));

    return R;
}

bool PumpController::isRunning(uint32_t currentMillis, int currentHumidity, int currentTemperature, int currentSoilMoisture, int currentLightIntensity)
{

    auto runtime = calculateRuntime(currentHumidity, currentTemperature, currentSoilMoisture, currentLightIntensity);
    if (runtime == 0)
    {
        lastRunMillis = currentMillis;
        return false;
    }

    auto nextRun = config.frequencyMillis + lastRunMillis;
    auto endNextRun = nextRun + runtime;

    if (currentMillis > nextRun && currentMillis < endNextRun)
    {
        return true;
    }
    else if (currentMillis > endNextRun)
    {
        lastRunMillis = currentMillis;
        isForceRun = false;
        return false;
    }
    else
    {
        return false;
    }
}

void PumpController::forceRun(uint32_t currentMillis)
{
    isForceRun = true;
    lastRunMillis = currentMillis - config.frequencyMillis;
}

void PumpController::forceStop(uint32_t currentMillis)
{
    isForceRun = false;
    lastRunMillis = currentMillis;
}

PumpController::~PumpController()
{
}
