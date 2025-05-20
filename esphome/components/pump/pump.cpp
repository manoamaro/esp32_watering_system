#include "pump.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace pump
    {
        static const char *const TAG = "pump";

        void Pump::loop()
        {
            if (!this->enabled_switch_->state) {
                return;
            }

            if (this->auto_mode_switch_->state)
            {

                uint32_t now = millis();
                int16_t frequency = this->frequency_number_->state * 1000; // Convert to milliseconds
                uint32_t next_run = this->last_run_time_ + frequency;
                if (now < next_run)
                {
                    return;
                }
                int16_t duration = this->duration_number_->state * 1000; // Convert to milliseconds
                uint32_t end_run = next_run + duration;

                int16_t curr_soil = this->curr_soil_();
                int16_t curr_light = this->curr_light_();
                int16_t curr_temperature = this->curr_temperature_();
                int16_t min_temperature = this->min_temperature_number_->state;
                int16_t min_light = this->min_light_number_->state;
                int16_t max_moisture = this->max_moisture_number_->state;

                // Check if the conditions are met to start the pump
                if (curr_soil < max_moisture && curr_light > min_light && curr_temperature > min_temperature && now <= end_run)
                {
                    if (this->pump_running_binary_sensor_->state == false) {
                        this->pump_running_binary_sensor_->publish_state(true);
                        this->pump_output_->turn_on();
                        ESP_LOGD(TAG, "Pump is running");
                    }
                }
                else
                {
                    ESP_LOGD(TAG, "Pump is stopping 1");
                    this->last_run_time_ = now;
                    this->pump_output_->turn_off();
                    this->pump_running_binary_sensor_->publish_state(false);
                }
            }
        }

        void Pump::dump_config()
        {
        }
    }
}