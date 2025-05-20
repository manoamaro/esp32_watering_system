#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/number/number.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/adc/adc_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/preferences.h"

namespace esphome
{
    namespace adc_soil_sensor
    {
        class AdcSoilSensor : public adc::ADCSensor
        {
            AdcSoilSensor() : adc::ADCSensor() {
                
            }
        };
    }
}