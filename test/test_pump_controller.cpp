#include <Arduino.h>
#include <unity.h>
#include <stdio.h>
#include "pump_controller.hpp"

const int humidity = 60;
const int temperature = 30;
const int soilMoisture = 200;
const int lightIntensity = 1000;

void test_calculate_humidity(void)
{
    auto pumpController = PumpController();

    for (int i = 0; i < 100; i++)
    {
        printf("Watering time: %d\n", pumpController.calculateRuntime(i, temperature, soilMoisture, lightIntensity));
    }
}

void test_calculate_temperature(void)
{
    auto pumpController = PumpController();

    for (int i = -20; i < 50; i++)
    {
        printf("Watering time: %d\n", pumpController.calculateRuntime(humidity, i, soilMoisture, lightIntensity));
    }
}

void test_calculate_zero_when_soil_above_threshold(void)
{
    auto pumpController = PumpController();
    pumpController.config.maxSoilMoistureTreshold = 500;

    TEST_ASSERT_EQUAL(0, pumpController.calculateRuntime(humidity, temperature, 600, lightIntensity));
}

void test_calculate_zero_when_light_below_threshold(void)
{
    auto pumpController = PumpController();
    pumpController.config.minLightTreshold = 100;

    TEST_ASSERT_EQUAL(0, pumpController.calculateRuntime(humidity, temperature, soilMoisture, 99));
}

void test_is_running(void)
{
    auto pumpController = PumpController();
    pumpController.config.baseRuntimeMillis = 60000;
    pumpController.config.baseRuntimeMillis = 10000;
    pumpController.lastRunMillis = 0;

    auto runtime = pumpController.calculateRuntime(humidity, temperature, soilMoisture, lightIntensity);

    TEST_ASSERT_EQUAL(0, pumpController.isRunning(0, humidity, temperature, soilMoisture, lightIntensity));
    TEST_ASSERT_EQUAL(1, pumpController.isRunning(60001, humidity, temperature, soilMoisture, lightIntensity));
    TEST_ASSERT_EQUAL(0, pumpController.isRunning(60000 + runtime, humidity, temperature, soilMoisture, lightIntensity));
    TEST_ASSERT_EQUAL(0, pumpController.isRunning(60000 + runtime + 60001, humidity, temperature, soilMoisture, lightIntensity));
}


void setUp(void)
{
    // Set up your testing objects here
}

void tearDown(void)
{
    // Clean up your testing objects here
}

void setup()
{
    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_calculate_humidity);
    RUN_TEST(test_calculate_temperature);
    RUN_TEST(test_calculate_zero_when_soil_above_threshold);
    RUN_TEST(test_calculate_zero_when_light_below_threshold);
    RUN_TEST(test_is_running);

    UNITY_END(); // stop unit testing
}

void loop()
{
}