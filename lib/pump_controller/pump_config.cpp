#include "pump_config.hpp"

PumpConfig::PumpConfig(uint8_t pumpIdx) : pumpIdx(pumpIdx)
{
    humidityWeight = -100;
    temperatureWeight = 100;
    soilMoistureWeight = 0;
    lightWeight = 100;
    maxSoilMoistureTreshold = 0;
    minLightTreshold = 0;
    maxHumidityThreshold = 100;
    minTemperatureThreshold = 0;
}

void PumpConfig::load()
{
    Preferences preferences;
    preferences.begin(PUMP_CONFIG_PREFS_NAMESPACE, true);
    char key[4];
    sprintf(key, "%d", pumpIdx);
    if (!preferences.isKey(key))
    {
        preferences.end();
        save();
        return;
    }
    else
    {
        preferences.getBytes(key, this, sizeof(PumpConfig));
    }
    preferences.end();
}

void PumpConfig::save()
{
    Preferences preferences;
    preferences.begin(PUMP_CONFIG_PREFS_NAMESPACE, false);
    char key[4];
    sprintf(key, "%d", pumpIdx);
    preferences.putBytes(key, this, sizeof(PumpConfig));
    preferences.end();
}

void PumpConfig::incHumidityWeight(int8_t by)
{
    humidityWeight += by;
    humidityWeight = constrain(humidityWeight, -100, 100);
}

void PumpConfig::incTemperatureWeight(int8_t by)
{
    temperatureWeight += by;
    temperatureWeight = constrain(temperatureWeight, -100, 100);
}

void PumpConfig::incSoilMoistureWeight(int8_t by)
{
    soilMoistureWeight += by;
    soilMoistureWeight = constrain(soilMoistureWeight, -100, 100);
}

void PumpConfig::incLightWeight(int8_t by)
{
    lightWeight += by;
    lightWeight = constrain(lightWeight, -100, 100);
}

void PumpConfig::incMinLightTreshold(int by)
{
    minLightTreshold += by;
    minLightTreshold = constrain(minLightTreshold, 0, 100);
}

void PumpConfig::incMinTemperatureThreshold(int by)
{
    minTemperatureThreshold += by;
    minTemperatureThreshold = constrain(minTemperatureThreshold, PUMP_MIN_TEMPERATURE, PUMP_MAX_TEMPERATURE);
}

void PumpConfig::incMaxSoilMoistureTreshold(int by)
{
    maxSoilMoistureTreshold += by;
    maxSoilMoistureTreshold = constrain(maxSoilMoistureTreshold, 0, 100);
}

void PumpConfig::incMaxHumidityThreshold(int by)
{
    maxHumidityThreshold += by;
    maxHumidityThreshold = constrain(maxHumidityThreshold, 0, 100);
}

void PumpConfig::incBaseRuntimeMillis(int by)
{
    baseRuntimeMillis += by;
    baseRuntimeMillis = constrain(baseRuntimeMillis, 0, PUMP_MAX_RUNTIME);
}

void PumpConfig::incFrequencyMillis(int by)
{
    frequencyMillis += by;
    frequencyMillis = constrain(frequencyMillis, 0, PUMP_MAX_FREQUENCY);
}

PumpConfig::~PumpConfig()
{
}