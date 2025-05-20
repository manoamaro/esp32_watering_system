#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/number/number.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/preferences.h"

namespace esphome
{
    namespace pump
    {
        class Switch : public switch_::Switch, public Component
        {
        public:
            Switch() : esphome::switch_::Switch() {}
            void setup() override
            {
                optional<bool> initial_state = this->get_initial_state_with_restore_mode();
                if (initial_state.has_value())
                {
                    if (initial_state.value())
                    {
                        this->turn_on();
                    }
                    else
                    {
                        this->turn_off();
                    }
                }
            }

        protected:
            void write_state(bool state) override { this->publish_state(state); }
        };

        class Number : public number::Number, public Component
        {
        public:
            Number() : esphome::number::Number() {}
            void setup() override
            {
                if (this->has_state())
                    return;
                float value;
                this->pref_ = global_preferences->make_preference<float>(this->get_object_id_hash());
                if (!this->pref_.load(&value))
                {
                    value = this->traits.get_min_value();
                }
                this->publish_state(value);
            }

        protected:
            ESPPreferenceObject pref_;
            void control(float value) override
            {
                this->publish_state(value);
                this->pref_.save(&value);
            }
        };

        class BinarySensor : public binary_sensor::BinarySensorInitiallyOff, public Component
        {
        public:
            BinarySensor() : binary_sensor::BinarySensorInitiallyOff() {}
        };

        typedef std::function<int16_t()> _lambda_int16_t;
        class Pump : public Component
        {
            SUB_SWITCH(enabled)
            SUB_SWITCH(auto_mode)
            SUB_BINARY_SENSOR(pump_running)
            SUB_NUMBER(min_temperature)
            SUB_NUMBER(max_moisture)
            SUB_NUMBER(min_light)
            SUB_NUMBER(duration)
            SUB_NUMBER(frequency)
        public:
            void setup() override
            {
            }
            void loop() override;
            void dump_config() override;
            float get_setup_priority() const override { return setup_priority::HARDWARE; }

            void set_pump_output(output::FloatOutput *pump_output)
            {
                pump_output_ = pump_output;
            }

            void set_curr_soil(_lambda_int16_t curr_soil)
            {
                curr_soil_ = curr_soil;
            }
            void set_curr_soil(int16_t curr_soil)
            {
                curr_soil_ = [curr_soil]()
                { return curr_soil; };
            }

            void set_curr_light(_lambda_int16_t curr_light)
            {
                curr_light_ = curr_light;
            }
            void set_curr_light(int16_t curr_light)
            {
                curr_light_ = [curr_light]()
                { return curr_light; };
            }

            void set_curr_temperature(_lambda_int16_t curr_temperature)
            {
                curr_temperature_ = curr_temperature;
            }
            void set_curr_temperature(int16_t curr_temperature)
            {
                curr_temperature_ = [curr_temperature]()
                { return curr_temperature; };
            }
        private:
            esphome::output::FloatOutput *pump_output_;
            _lambda_int16_t curr_soil_;
            _lambda_int16_t curr_light_;
            _lambda_int16_t curr_temperature_;
            uint32_t last_run_time_;
            uint32_t last_check_time_;
        };

    } // namespace pump
} // namespace esphome
