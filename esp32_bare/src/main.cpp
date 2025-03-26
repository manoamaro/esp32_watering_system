#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Adafruit_BME280.h>
#include "ui.h"
#include "WiFi.h"
#include "time.h"
#include "pump_controller.hpp"
#include "analog_sensor.hpp"
#include <WiFiProvisioner.h>
#include <utils.hpp>
#include "firebase.hpp"

/*************************************************************
 * Definitions
 ************************************************************/
#define DEBUG 1 // Set DEBUG to 1 to enable debug printing; set to 0 to disable
#if DEBUG == 1
#define DEBUG_PRINTLN(x) Serial.println(F(x))
#else
#define DEBUG_PRINTLN(x) // Completely removes the debug code when DEBUG is 0
#endif
#define TFT_HOR_RES 240
#define TFT_VER_RES 240
#define DISPLAY_BRIGTHNESS 128
#ifdef DEBUG
#define SLEEP_TIMEOUT 1000 * 60 * 60
#else
#define SLEEP_TIMEOUT 30000
#endif
#define MAX_PUMP_POWER 200
#define LCD_LED_PIN GPIO_NUM_27
#define LIGHT_SENSOR_PIN GPIO_NUM_36
#define SOIL_MOISTURE_1_SENSOR_PIN GPIO_NUM_39
#define SOIL_MOISTURE_2_SENSOR_PIN GPIO_NUM_34
#define SOIL_MOISTURE_3_SENSOR_PIN GPIO_NUM_35
#define PUMP_1_PIN GPIO_NUM_12
#define PUMP_2_PIN GPIO_NUM_4
#define PUMP_3_PIN GPIO_NUM_14
#define BUTTON_1_PIN GPIO_NUM_13
#define BUTTON_2_PIN GPIO_NUM_15
#define BUTTON_3_PIN GPIO_NUM_17
#define PUMP_COUNT 3
#define TEMP_SENSOR_OFFSET -6


/*************************************************************
 * Prototypes
 ************************************************************/
/* lvgl initialization */
void init_lvgl();
void init_ui();
uint32_t my_tick_get_cb(void);

/* RTOS Tasks */
void initWifi(void *pvParameters);
void readSensors(void *pvParameters);
void updateLgvl(void *pvParameters);
void checkPump(void *pvParameters);
void onClockTimer(lv_timer_t *timer);

/* Events */
void onWiFiEvent(WiFiEvent_t event);
void onButtonRead(lv_indev_t *indev, lv_indev_data_t *data);
void IRAM_ATTR onButtonPressed();
void IRAM_ATTR onButtonReleased();

/* Utils */
lv_obj_t *getPumpUI(uint8_t pumpIdx);
void startWifi();

/*************************************************************
 * Global Variables
 ************************************************************/
// RTOS Tasks
TaskHandle_t readSensorsTask;
TaskHandle_t updateLgvlTask;
TaskHandle_t initWifiTask;
TaskHandle_t checkPumpTask;

// WiFi and NTP
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 1;
const int daylightOffset_sec = 3600 * 0;
struct tm timeinfo;

volatile int8_t selectedPump = 0;
PumpController pumpControllers[PUMP_COUNT] = {
    PumpController(0),
    PumpController(1),
    PumpController(2)};
Adafruit_BME280 bme;
volatile float temperature, humidity, pressure;
AnalogSensor lightSensor(LIGHT_SENSOR_PIN);
AnalogSensor soilSensors[PUMP_COUNT] = {
    AnalogSensor(SOIL_MOISTURE_1_SENSOR_PIN),
    AnalogSensor(SOIL_MOISTURE_2_SENSOR_PIN),
    AnalogSensor(SOIL_MOISTURE_3_SENSOR_PIN)};

lv_indev_t *buttonIndev = NULL;

// WifiManager wifiManager;
WiFiProvisioner::WiFiProvisioner wifiProvisioner;

char *deviceUid = utils::getMacAddress();
Firebase firebase;

void setup()
{
#if DEBUG == 1
    Serial.begin(115200);
#endif
    DEBUG_PRINTLN("Init");

    ledcSetup(0, 5000, 8);
    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);
    ledcSetup(3, 5000, 8);

    pinMode(LCD_LED_PIN, OUTPUT);
    ledcAttachPin(LCD_LED_PIN, 0);
    ledcWrite(0, DISPLAY_BRIGTHNESS);

    pinMode(PUMP_1_PIN, OUTPUT);
    ledcAttachPin(PUMP_1_PIN, 1);
    pinMode(PUMP_2_PIN, OUTPUT);
    ledcAttachPin(PUMP_2_PIN, 2);
    pinMode(PUMP_3_PIN, OUTPUT);
    ledcAttachPin(PUMP_3_PIN, 3);

    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
    pinMode(BUTTON_3_PIN, INPUT_PULLUP);

    attachInterrupt(BUTTON_1_PIN, onButtonPressed, FALLING);
    // attachInterrupt(BUTTON_1_PIN, onButtonReleased, RISING);
    attachInterrupt(BUTTON_2_PIN, onButtonPressed, FALLING);
    // attachInterrupt(BUTTON_2_PIN, onButtonReleased, RISING);
    attachInterrupt(BUTTON_3_PIN, onButtonPressed, FALLING);
    // attachInterrupt(BUTTON_3_PIN, onButtonReleased, RISING);

    init_lvgl();
    init_ui();

    if (!bme.begin(0x76))
    {
        DEBUG_PRINTLN("Could not find a valid BME280 sensor, check wiring!");
        // delay(100);
    }

    DEBUG_PRINTLN("BME280 sensor found!");

    startWifi();
    xTaskCreate(readSensors, "readSensors", 4096, NULL, 1, &readSensorsTask);
    xTaskCreate(updateLgvl, "updateLgvl", 1024 * 16, NULL, 3, &updateLgvlTask);
    xTaskCreate(checkPump, "checkPump", 4096, NULL, 2, &checkPumpTask);
    lv_timer_t *timer = lv_timer_create(onClockTimer, 1000, NULL);

    for (uint8_t pumpIdx = 0; pumpIdx < PUMP_COUNT; pumpIdx++)
    {
        auto pumpController = &pumpControllers[pumpIdx];
        pumpController->config.load();
        soilSensors[pumpIdx].setmax(pumpController->config.soilSensorDryValue);
        soilSensors[pumpIdx].setmin(pumpController->config.soilSensorWetValue);
    }

    
}

void loop()
{
}

void init_lvgl()
{
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10)
    static uint8_t draw_buf[DRAW_BUF_SIZE];
    static lv_display_t *disp;
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, LV_DISP_ROTATION_270);
    lv_display_set_default(disp);

    static const lv_point_t btn_points[3] = {
        {40, 220}, {119, 220}, {200, 220}};
    buttonIndev = lv_indev_create();
    lv_indev_set_type(buttonIndev, LV_INDEV_TYPE_BUTTON);
    // lv_indev_set_mode(buttonIndev, LV_INDEV_MODE_EVENT);
    lv_indev_set_read_cb(buttonIndev, onButtonRead);
    lv_indev_set_button_points(buttonIndev, btn_points);
}

void init_ui()
{
    DEBUG_PRINTLN("Init UI");
    lv_lock();
    lv_disp_t *disp = lv_display_get_default();
    if (!disp)
    {
        return;
    }
    ui_init();

    lv_label_set_text(ui_comp_get_child(ui_Main_Pump1Container, UI_COMP_PUMPCONTAINER_LABELCONTAINER_LABEL), "1");
    lv_label_set_text(ui_comp_get_child(ui_Main_Pump2Container, UI_COMP_PUMPCONTAINER_LABELCONTAINER_LABEL), "2");
    lv_label_set_text(ui_comp_get_child(ui_Main_Pump3Container, UI_COMP_PUMPCONTAINER_LABELCONTAINER_LABEL), "3");
    lv_obj_add_state(ui_Main_Pump1Container, LV_STATE_FOCUSED);

    lv_unlock();
    DEBUG_PRINTLN("UI Initialized");
}

void initWifi(void *pvParameters)
{
    wifiProvisioner.connectToWiFi();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    initWifiTask = NULL;
    vTaskDelete(NULL);
}

uint32_t my_tick_get_cb(void) { return millis(); }

float calculateAbsoluteHumidity(float temperatureC, float relativeHumidity)
{
    // Constants used in the Magnus formula for water vapor pressure
    float es = 6.112 * exp((17.67 * temperatureC) / (temperatureC + 243.5));             // Saturation vapor pressure in hPa
    float absoluteHumidity = (es * relativeHumidity * 2.1674) / (temperatureC + 273.15); // g/m³
    return absoluteHumidity;
}

float calculateRelativeHumidity(float temperatureC, float absoluteHumidity)
{
    // Constants used in the Magnus formula for water vapor pressure
    float es = 6.112 * exp((17.67 * temperatureC) / (temperatureC + 243.5));               // Saturation vapor pressure in hPa
    float relativeHumidity = (absoluteHumidity * (temperatureC + 273.15)) / (es * 2.1674); // %
    return relativeHumidity;
}

void readSensors(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500);
    while (1)
    {
        auto bmeTemp = bme.readTemperature();
        auto bmeHumidity = bme.readHumidity();
        pressure = bme.readPressure();

        auto absoluteHumidity = calculateAbsoluteHumidity(bmeTemp, bmeHumidity);
        temperature = bmeTemp + TEMP_SENSOR_OFFSET;
        humidity = calculateRelativeHumidity(temperature, absoluteHumidity);

        for (uint8_t pumpIdx = 0; pumpIdx < PUMP_COUNT; pumpIdx++)
        {
            auto v = soilSensors[pumpIdx].read();
        }
        lightSensor.read();
        static char buf[10];
        lv_lock();
        sprintf(buf, "%.1f°C", temperature);
        lv_label_set_text(ui_Main_tempLabel, buf);
        sprintf(buf, "%.1f%%", humidity);
        lv_label_set_text(ui_Main_humidityLabel, buf);
        lv_unlock();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void updateLgvl(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t delay;
    while (1)
    {
        if (lv_display_get_inactive_time(NULL) < SLEEP_TIMEOUT)
        {
            // DEBUG_PRINTLN("Updating lvgl");
            delay = lv_timer_handler();
            if (delay > 0)
            {
                xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(delay));
            }
        }
        else
        {
            // fade out the display backlight
            for (uint8_t i = DISPLAY_BRIGTHNESS; i > 0; i--)
            {
                ledcWrite(0, i);
                vTaskDelay(pdMS_TO_TICKS(5));
            }
            ledcWrite(0, 0);
            DEBUG_PRINTLN("Going to sleep");
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            DEBUG_PRINTLN("Waking up");
            lv_tick_inc(LV_DEF_REFR_PERIOD);
            lv_task_handler();
            // fade in the display backlight
            for (uint8_t i = 0; i <= DISPLAY_BRIGTHNESS; i++)
            {
                ledcWrite(0, i);
                vTaskDelay(pdMS_TO_TICKS(5));
            }
        }
    }
}

void checkPump(void *pvParameters)
{
    while (true)
    {
        for (uint8_t pumpIdx = 0; pumpIdx < PUMP_COUNT; pumpIdx++)
        {
            auto pumpController = &pumpControllers[pumpIdx];
            auto pumpContainer = getPumpUI(pumpIdx);
            auto soilSensor = &soilSensors[pumpIdx];
            auto lightSensorValue = lightSensor.getValuePercentage();
            auto soilSensorValue = soilSensor->getInvertedValuePercentage();

            lv_lock();

            auto pumpRunning = ui_comp_get_child(pumpContainer, UI_COMP_PUMPCONTAINER_LABELCONTAINER_RUNNING);
            auto pumpProgress = ui_comp_get_child(pumpContainer, UI_COMP_PUMPCONTAINER_PROGRESS);
            auto pumpSoil = ui_comp_get_child(pumpContainer, UI_COMP_PUMPCONTAINER_SOIL);
            auto pumpLight = ui_comp_get_child(pumpContainer, UI_COMP_PUMPCONTAINER_LIGHT);

            lv_obj_set_state(pumpContainer, LV_STATE_DISABLED, !pumpController->config.enabled);
            lv_obj_set_state(pumpRunning, LV_STATE_DISABLED, !pumpController->config.enabled);
            lv_obj_set_state(pumpProgress, LV_STATE_DISABLED, !pumpController->config.enabled);
            lv_obj_set_state(pumpSoil, LV_STATE_DISABLED, !pumpController->config.enabled);
            lv_obj_set_state(pumpLight, LV_STATE_DISABLED, !pumpController->config.enabled);

            if (pumpController->config.enabled)
            {

                lv_arc_set_value(pumpSoil, soilSensorValue);
                lv_arc_set_value(pumpLight, lightSensorValue);

                if (lightSensorValue >= pumpController->config.minLightTreshold)
                {
                    lv_obj_add_state(pumpLight, LV_STATE_USER_1);
                    lv_obj_remove_state(pumpLight, LV_STATE_USER_2);
                }
                else
                {
                    lv_obj_add_state(pumpLight, LV_STATE_USER_2);
                    lv_obj_remove_state(pumpLight, LV_STATE_USER_1);
                }

                if (soilSensorValue <= pumpController->config.maxSoilMoistureTreshold)
                {
                    lv_obj_add_state(pumpSoil, LV_STATE_USER_1);
                    lv_obj_remove_state(pumpSoil, LV_STATE_USER_2);
                }
                else
                {
                    lv_obj_add_state(pumpSoil, LV_STATE_USER_2);
                    lv_obj_remove_state(pumpSoil, LV_STATE_USER_1);
                }

                if (pumpController->isRunning(millis(), humidity, temperature, soilSensorValue, lightSensorValue))
                {
                    // Turn on pump
                    ledcWrite(pumpIdx + 1, MAX_PUMP_POWER);
                    lv_obj_update_flag(pumpRunning, LV_OBJ_FLAG_HIDDEN, false);
                    // Update progress bar with remaining pump time
                    auto runtime = pumpController->calculateRuntime(humidity, temperature, soilSensorValue, lightSensorValue);
                    auto nextRun = pumpController->lastRunMillis + pumpController->config.frequencyMillis;
                    auto remaining = nextRun + runtime - millis();
                    lv_bar_set_value(pumpProgress, map(remaining, 0, runtime, 0, 100), LV_ANIM_ON);
                }
                else
                {
                    // Turn off pump
                    ledcWrite(pumpIdx + 1, 0);
                    lv_obj_update_flag(pumpRunning, LV_OBJ_FLAG_HIDDEN, true);
                    // Update progress bar with time for next pump
                    auto nextRun = pumpController->lastRunMillis + pumpController->config.frequencyMillis;
                    auto remaining = nextRun - millis();
                    lv_bar_set_value(pumpProgress, map(remaining, 0, pumpController->config.frequencyMillis, 0, 100), LV_ANIM_ON);
                }

                if (pumpController->shouldRun(humidity, temperature, soilSensorValue, lightSensorValue))
                {
                    lv_obj_remove_state(pumpProgress, LV_STATE_DISABLED);
                }
                else
                {
                    lv_obj_add_state(pumpProgress, LV_STATE_DISABLED);
                }
            }
            else
            {
                lv_arc_set_value(pumpSoil, 0);
                lv_arc_set_value(pumpLight, 0);
                lv_spinner_set_anim_params(pumpRunning, 0, 0);
            }

            lv_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void onClockTimer(lv_timer_t *timer)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        getLocalTime(&timeinfo, 0);
    }
    static char buf[64];
    // Info header
    strftime(buf, sizeof(buf), "%a. %e %b %Y, %T", &timeinfo);
    lv_label_set_text(ui_Main_datetime, buf);
}

lv_obj_t *getPumpUI(uint8_t pumpIdx)
{
    switch (pumpIdx)
    {
    case 0:
        return ui_Main_Pump1Container;
        break;
    case 1:
        return ui_Main_Pump2Container;
        break;
    case 2:
        return ui_Main_Pump3Container;
        break;
    default:
        return NULL;
        break;
    }
}

void onWiFiEvent(WiFiEvent_t event)
{
    lv_lock();
    switch (event)
    {
    case SYSTEM_EVENT_STA_CONNECTED:
        DEBUG_PRINTLN("Connected to WiFi network");
        lv_img_set_src(ui_Main_connImg, &ui_img_wifi_png);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        DEBUG_PRINTLN("Disconnected from WiFi network");
        lv_img_set_src(ui_Main_connImg, &ui_img_no_wifi_png);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        DEBUG_PRINTLN("Got IP address");
        break;
    default:
        break;
    }
    lv_unlock();
}

void IRAM_ATTR onButtonPressed()
{
    vTaskNotifyGiveFromISR(updateLgvlTask, NULL);
    // lv_indev_read(buttonIndev);
}

void IRAM_ATTR onButtonReleased()
{
    vTaskNotifyGiveFromISR(updateLgvlTask, NULL);
}

void onButtonRead(lv_indev_t *indev, lv_indev_data_t *data)
{
    /*Store the last pressed button*/
    static uint32_t last_btn = 0;
    int btn_pr = -1;
    if (digitalRead(BUTTON_1_PIN) == LOW)
    {
        btn_pr = 0;
    }
    else if (digitalRead(BUTTON_2_PIN) == LOW)
    {
        btn_pr = 1;
    }
    else if (digitalRead(BUTTON_3_PIN) == LOW)
    {
        btn_pr = 2;
    }
    /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
    if (btn_pr >= 0)
    {
        /*Save the ID of the pressed button*/
        last_btn = btn_pr;
        /*Set the pressed state*/
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        /*Set the released state*/
        data->state = LV_INDEV_STATE_RELEASED;
    }
    /*Save the last button*/
    data->btn_id = last_btn;
}

void startWifi()
{
    DEBUG_PRINTLN("Starting WiFi");
    if (initWifiTask == NULL)
    {
        xTaskCreate(initWifi, "initWifi", 4096, NULL, 1, &initWifiTask);
    }
    else
    {
        DEBUG_PRINTLN("WiFi task already running");
    }
}

/*************************************************************
 * Main Screen
 ************************************************************/

void onMainScreenLoaded(lv_event_t *e)
{
    WiFi.onEvent(onWiFiEvent);
}

void onMainScreenUnloaded(lv_event_t *e)
{
    WiFi.removeEvent(onWiFiEvent);
}

void onRunButtonClicked(lv_event_t *e)
{
    if (selectedPump >= 0)
    {
        auto pumpController = &pumpControllers[selectedPump];
        if (pumpController->config.enabled)
        {
            if (pumpController->isRunning(millis(), humidity, temperature, soilSensors[selectedPump].getValuePercentage(), lightSensor.getValuePercentage()))
            {
                pumpController->forceStop(millis());
            }
            else
            {
                pumpController->forceRun(millis());
            }
        }
    }
}

void onPumpButtonClicked(lv_event_t *e)
{
    auto pumpContainer = getPumpUI(selectedPump);
    lv_obj_remove_state(pumpContainer, LV_STATE_FOCUSED);
    selectedPump = (selectedPump + 1) % PUMP_COUNT;
    pumpContainer = getPumpUI(selectedPump);
    lv_obj_add_state(pumpContainer, LV_STATE_FOCUSED);
}

/*************************************************************
 * Settings Screen
 ************************************************************/
struct SettingsScreen
{
    int8_t mainOptionsCount = 5;
    int8_t selectedMainOption = 0;
    int8_t selectedPumpIdx = 0;
} settingsScreen;

void onSettingsScreenLoaded(lv_event_t *e)
{
    settingsScreen.selectedMainOption = 0;
    settingsScreen.selectedPumpIdx = 0;
    lv_roller_set_selected(ui_Settings_options, settingsScreen.selectedMainOption, LV_ANIM_OFF);
}

void onSettingsUpButtonClicked(lv_event_t *e)
{
    settingsScreen.selectedMainOption = constrain(settingsScreen.selectedMainOption - 1, 0, settingsScreen.mainOptionsCount - 1);
    lv_roller_set_selected(ui_Settings_options, settingsScreen.selectedMainOption, LV_ANIM_ON);
}

void onSettingsDownClicked(lv_event_t *e)
{
    settingsScreen.selectedMainOption = constrain(settingsScreen.selectedMainOption + 1, 0, settingsScreen.mainOptionsCount - 1);
    lv_roller_set_selected(ui_Settings_options, settingsScreen.selectedMainOption, LV_ANIM_ON);
}

void onSettingsEnterClicked(lv_event_t *e)
{
    switch (settingsScreen.selectedMainOption)
    {
    case 0:
        // Open PumpConfig screen with pump 1
        settingsScreen.selectedPumpIdx = 0;
        _ui_screen_change(&ui_PumpConfig, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_PumpConfig_screen_init);
        break;
    case 1:
        // Open PumpConfig screen with pump 2
        settingsScreen.selectedPumpIdx = 1;
        _ui_screen_change(&ui_PumpConfig, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_PumpConfig_screen_init);
        break;
    case 2:
        // Open PumpConfig screen with pump 3
        settingsScreen.selectedPumpIdx = 2;
        _ui_screen_change(&ui_PumpConfig, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_PumpConfig_screen_init);
        break;
    case 3:
        // Open WiFiSettings screen
        _ui_screen_change(&ui_WiFiSettings, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_WiFiSettings_screen_init);
        break;
    case 4:
        _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Main_screen_init);
        break;
    default:
        break;
    }
}

/*************************************************************
 * PumpConfig Screen
 ************************************************************/
lv_group_t *pumpConfigOptionsGroup;

void displayPumpConfig()
{
    auto config = &pumpControllers[settingsScreen.selectedPumpIdx].config;
    lv_label_set_text_fmt(ui_PumpConfig_titleLabel, "Pump %d Config", settingsScreen.selectedPumpIdx + 1);
    lv_obj_set_state(ui_PumpConfig_enabledSwitch, LV_STATE_CHECKED, config->enabled);
    lv_label_set_text_fmt(ui_PumpConfig_frequencyValue, "%02d:%02d", (config->frequencyMillis / 1000) / 60, (config->frequencyMillis / 1000) % 60);
    lv_label_set_text_fmt(ui_PumpConfig_runtimeValue, "%02d:%02d", (config->baseRuntimeMillis / 1000) / 60, (config->baseRuntimeMillis / 1000) % 60);
    lv_label_set_text_fmt(ui_PumpConfig_minLightThresholdValue, "%02d%%", config->minLightTreshold);
    lv_label_set_text_fmt(ui_PumpConfig_maxSoilThresholdValue, "%02d%%", config->maxSoilMoistureTreshold);
    lv_label_set_text_fmt(ui_PumpConfig_minTempThresholdValue, "%02d°C", config->minTemperatureThreshold);
    lv_label_set_text_fmt(ui_PumpConfig_maxHumidityThresholdValue, "%02d%%", config->maxHumidityThreshold);
    lv_label_set_text_fmt(ui_PumpConfig_soilSensorDryValue, "%d", config->soilSensorDryValue);
    lv_label_set_text_fmt(ui_PumpConfig_soilSensorWetValue, "%d", config->soilSensorWetValue);

    if (lv_group_get_editing(pumpConfigOptionsGroup))
    {
        auto focusedObj = lv_group_get_focused(pumpConfigOptionsGroup);
        if (focusedObj == ui_PumpConfig_enabledPanel)
        {
            lv_label_set_text(ui_PumpConfig_btn1Label, "Toggle");
            lv_label_set_text(ui_PumpConfig_btn2Label, "Toggle");
            lv_label_set_text(ui_PumpConfig_btn3Label, "Back");
        }
        else if (focusedObj == ui_PumpConfig_soilSensorDryPanel || focusedObj == ui_PumpConfig_soilSensorWetPanel)
        {
            lv_label_set_text(ui_PumpConfig_btn1Label, "Read");
            lv_label_set_text(ui_PumpConfig_btn2Label, "Read");
            lv_label_set_text(ui_PumpConfig_btn3Label, "Back");
        }
        else
        {
            lv_label_set_text(ui_PumpConfig_btn1Label, "+");
            lv_label_set_text(ui_PumpConfig_btn2Label, "-");
            lv_label_set_text(ui_PumpConfig_btn3Label, "Back");
        }
    }
    else
    {
        lv_label_set_text(ui_PumpConfig_btn1Label, "Up/Save");
        lv_label_set_text(ui_PumpConfig_btn2Label, "Down");
        lv_label_set_text(ui_PumpConfig_btn3Label, "Edit");
    }
}

void incPumpConfigValue(uint8_t pumpIdx, uint8_t configIdx, int by)
{
    auto pumpController = &pumpControllers[pumpIdx];
    switch (configIdx)
    {
    case 0:
        pumpController->config.enabled = !pumpController->config.enabled;
        break;
    case 1:
        pumpController->config.incFrequencyMillis(by);
        break;
    case 2:
        pumpController->config.incBaseRuntimeMillis(by);
        break;
    case 3:
        pumpController->config.incMinLightTreshold(by);
        break;
    case 4:
        pumpController->config.incMaxSoilMoistureTreshold(by);
        break;
    case 5:
        pumpController->config.incMinTemperatureThreshold(by);
        break;
    case 6:
        pumpController->config.incMaxHumidityThreshold(by);
        break;
    default:
        break;
    }
}

void incPumpConfig(bool inc, bool fast)
{
    int mult = inc ? 1 : -1;
    mult *= fast ? 10 : 1;
    auto focusedObj = lv_group_get_focused(pumpConfigOptionsGroup);
    auto pumpIdx = settingsScreen.selectedPumpIdx;
    if (focusedObj == ui_PumpConfig_enabledPanel)
    {
        incPumpConfigValue(pumpIdx, 0, inc ? 1 : 0);
    }
    else if (focusedObj == ui_PumpConfig_frequencyPanel)
    {
        incPumpConfigValue(pumpIdx, 1, 1000 * mult);
    }
    else if (focusedObj == ui_PumpConfig_runtimePanel)
    {
        incPumpConfigValue(pumpIdx, 2, 1000 * mult);
    }
    else if (focusedObj == ui_PumpConfig_minLightThresholdPanel)
    {
        incPumpConfigValue(pumpIdx, 3, 1 * mult);
    }
    else if (focusedObj == ui_PumpConfig_maxSoilThresholdPanel)
    {
        incPumpConfigValue(pumpIdx, 4, 1 * mult);
    }
    else if (focusedObj == ui_PumpConfig_minTempThresholdPanel)
    {
        incPumpConfigValue(pumpIdx, 5, 1 * mult);
    }
    else if (focusedObj == ui_PumpConfig_maxHumidityThresholdPanel)
    {
        incPumpConfigValue(pumpIdx, 6, 1 * mult);
    }
    else if (focusedObj == ui_PumpConfig_soilSensorDryPanel)
    {
        pumpControllers[pumpIdx].config.soilSensorDryValue = soilSensors[pumpIdx].read();
    }
    else if (focusedObj == ui_PumpConfig_soilSensorWetPanel)
    {
        pumpControllers[pumpIdx].config.soilSensorWetValue = soilSensors[pumpIdx].read();
    }
}

void onPumpConfigBtn1Clicked(lv_event_t *e)
{
    if (lv_group_get_editing(pumpConfigOptionsGroup))
    {
        incPumpConfig(true, false);
        displayPumpConfig();
    }
    else
    {
        lv_group_focus_prev(pumpConfigOptionsGroup);
    }
}

void onPumpConfigBtn1LongPressed(lv_event_t *e)
{
    if (lv_group_get_editing(pumpConfigOptionsGroup))
    {
        incPumpConfig(true, true);
        displayPumpConfig();
    }
    else
    {
        // Persist the pump config
        auto pumpController = &pumpControllers[settingsScreen.selectedPumpIdx];
        pumpController->config.save();
        soilSensors[settingsScreen.selectedPumpIdx].setmax(pumpController->config.soilSensorDryValue);
        soilSensors[settingsScreen.selectedPumpIdx].setmin(pumpController->config.soilSensorWetValue);
        _ui_screen_change(&ui_Settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Settings_screen_init);
    }
}

void onPumpConfigBtn2Clicked(lv_event_t *e)
{
    if (lv_group_get_editing(pumpConfigOptionsGroup))
    {
        incPumpConfig(false, false);
        displayPumpConfig();
    }
    else
    {
        lv_group_focus_next(pumpConfigOptionsGroup);
    }
}

void onPumpConfigBtn2LongPressed(lv_event_t *e)
{
    if (lv_group_get_editing(pumpConfigOptionsGroup))
    {
        incPumpConfig(false, true);
        displayPumpConfig();
    }
    else
    {
        // Persist the pump config
        auto pumpController = &pumpControllers[settingsScreen.selectedPumpIdx];
        pumpController->config.load();
        _ui_screen_change(&ui_Settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Settings_screen_init);
    }
}

void onPumpConfigBtn3Clicked(lv_event_t *e)
{
    lv_group_set_editing(pumpConfigOptionsGroup, !lv_group_get_editing(pumpConfigOptionsGroup));
    displayPumpConfig();
}

void onPumpScreenLoaded(lv_event_t *e)
{
    if (pumpConfigOptionsGroup == NULL)
    {
        pumpConfigOptionsGroup = lv_group_create();
    }

    lv_group_remove_all_objs(pumpConfigOptionsGroup);
    // add all children of the pumpConfigOptions to the group
    for (int i = 0; i < lv_obj_get_child_cnt(ui_PumpConfig_content); i++)
    {
        lv_group_add_obj(pumpConfigOptionsGroup, lv_obj_get_child(ui_PumpConfig_content, i));
    }
    lv_group_set_editing(pumpConfigOptionsGroup, false);
    displayPumpConfig();
    lv_group_focus_obj(ui_PumpConfig_enabledPanel);
}

/*************************************************************
 * WiFi Settings Screen
 ************************************************************/

lv_obj_t *ui_WiFiSettings_QrCode;
static String qrCodeDataWiFiFmt = F("WIFI:T:WPA;S:%s;P:%s;;");

void onWifiSettingsEvent(WiFiEvent_t event)
{
    auto ssid = WiFi.SSID();
    auto ip = WiFi.localIP();
    char buf[128];

    lv_lock();
    Serial.printf("WiFi event: %d\n", event);
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        lv_label_set_text_fmt(ui_WiFiSettings_ssidLabel, "Connecting to %s", ssid.c_str());
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        lv_label_set_text_fmt(ui_WiFiSettings_ssidLabel, "Connected to %s", ssid.c_str());
        break;
    case SYSTEM_EVENT_AP_STOP:
    case SYSTEM_EVENT_STA_DISCONNECTED:
        lv_label_set_text(ui_WiFiSettings_ssidLabel, "Not connected");
        lv_label_set_text(ui_WiFiSettings_ipLabel, "");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        lv_label_set_text(ui_WiFiSettings_ipLabel, ip.toString().c_str());
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        lv_label_set_text(ui_WiFiSettings_ssidLabel, "Configuring WiFi");
        lv_obj_update_flag(ui_WiFiSettings_QrCode, LV_OBJ_FLAG_HIDDEN, false);
        // sprintf(buf, qrCodeDataWiFiFmt.c_str(), wifiManager.ssid.c_str(), wifiManager.password.c_str());
        lv_qrcode_update(ui_WiFiSettings_QrCode, buf, strlen(buf));
        break;
    default:
        break;
    }
    lv_unlock();
}

void onWifiSettingsLoaded(lv_event_t *e)
{
    WiFi.onEvent(onWifiSettingsEvent);
    auto ssid = WiFi.SSID();
    auto ip = WiFi.localIP();
    if (WiFi.isConnected())
    {
        lv_label_set_text(ui_WiFiSettings_ssidLabel, ssid.c_str());
        lv_label_set_text(ui_WiFiSettings_ipLabel, ip.toString().c_str());
    }
    else
    {
        lv_label_set_text(ui_WiFiSettings_ssidLabel, "Not connected");
        lv_label_set_text(ui_WiFiSettings_ipLabel, "");
    }

    if (ui_WiFiSettings_QrCode == NULL)
    {
        ui_WiFiSettings_QrCode = lv_qrcode_create(ui_WiFiSettings_content);
        lv_qrcode_set_size(ui_WiFiSettings_QrCode, 120);
    }
}

void onWifiSettingsUnloaded(lv_event_t *e)
{
    WiFi.removeEvent(onWifiSettingsEvent);
}

void onWifiScreenBtn1Clicked(lv_event_t *e)
{
    _ui_screen_change(&ui_Settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Settings_screen_init);
}

void onWifiScreenBtn2Clicked(lv_event_t *e)
{
}

void onWifiScreenBtn3Clicked(lv_event_t *e)
{
    wifiProvisioner.resetCredentials();
    startWifi();
}
