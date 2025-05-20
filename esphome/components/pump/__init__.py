import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import output, switch, number, binary_sensor

from esphome.const import (
    CONF_ID,
    CONF_AUTO_MODE,
    CONF_RUN_DURATION,
    CONF_FREQUENCY,
    ENTITY_CATEGORY_CONFIG,
    CONF_NAME,
)

AUTO_LOAD = ["number", "switch", "binary_sensor", "output"]
CODEOWNERS = ["@manoamaro"]

CONF_PUMP_ENABLED_SWITCH = "enabled_switch"
CONF_PUMP_OUTPUT = "pump_output"
CONF_CURR_SOIL = "curr_soil_sensor_value"
CONF_CURR_LIGHT = "curr_light_sensor_value"
CONF_CURR_TEMPERATURE = "curr_temperature_value"
CONF_MIN_TEMPERATURE = "min_temperature"
CONF_MAX_MOISTURE = "max_moisture"
CONF_MIN_LIGHT = "min_light"
CONF_PUMP_RUNNING = "running_sensor"

pump_ns = cg.esphome_ns.namespace("pump")
Pump = pump_ns.class_("Pump", cg.Component)
Switch = pump_ns.class_("Switch", switch.Switch, cg.Component)
Number = pump_ns.class_("Number", number.Number, cg.Component)
BinarySensor = pump_ns.class_("BinarySensor", binary_sensor.BinarySensor, cg.Component)

PUMP_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Pump),
        cv.Optional(CONF_PUMP_ENABLED_SWITCH, {CONF_NAME: "Enabled"}): switch.switch_schema(
            Switch,
            default_restore_mode="RESTORE_DEFAULT_OFF",
        ),
        cv.Required(CONF_PUMP_OUTPUT): cv.use_id(output.FloatOutput),
        cv.Required(CONF_CURR_SOIL): cv.Any(
            cv.templatable(cv.percentage_int),
            cv.returning_lambda
        ),
        cv.Required(CONF_CURR_LIGHT): cv.Any(
            cv.templatable(cv.percentage_int),
            cv.returning_lambda
        ),
        cv.Required(CONF_CURR_TEMPERATURE): cv.Any(
            cv.templatable(cv.temperature),
            cv.returning_lambda
        ),
        cv.Optional(CONF_MIN_TEMPERATURE, {CONF_NAME: "Min Temperature"}): number.number_schema(
            Number,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement="°C",
            device_class="temperature",
        ),
        cv.Optional(CONF_MAX_MOISTURE, {CONF_NAME: "Max Soil Moisture"}): number.number_schema(
            Number,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement="%",
            device_class="moisture",
        ),
        cv.Optional(CONF_MIN_LIGHT, {CONF_NAME: "Min Light"}): number.number_schema(
            Number,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement="%",
            device_class="illuminance",
        ),
        cv.Optional(CONF_RUN_DURATION, {CONF_NAME: "Run Duration"}): number.number_schema(
            Number,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement="s",
            device_class="duration",
        ),
        cv.Optional(CONF_FREQUENCY, {CONF_NAME: "Frequency"}): number.number_schema(
            Number,
            entity_category=ENTITY_CATEGORY_CONFIG,
            unit_of_measurement="s",
            device_class="frequency",
        ),
        cv.Optional(CONF_AUTO_MODE, {CONF_NAME: "Auto Mode"}): switch.switch_schema(
            Switch,
            default_restore_mode="RESTORE_DEFAULT_OFF",
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_PUMP_RUNNING, {CONF_NAME: "Running"}): binary_sensor.binary_sensor_schema(
            BinarySensor,
            device_class="running",
        ),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.ensure_list(PUMP_SCHEMA),
)

async def to_code(config):
    for pump_config in config:
        var = cg.new_Pvariable(pump_config[CONF_ID])
        await cg.register_component(var, pump_config)

        enable_switch_var = await switch.new_switch(pump_config[CONF_PUMP_ENABLED_SWITCH])
        await cg.register_component(enable_switch_var, pump_config[CONF_PUMP_ENABLED_SWITCH])
        cg.add(var.set_enabled_switch(enable_switch_var))
        
        pump_output = await cg.get_variable(pump_config[CONF_PUMP_OUTPUT])
        cg.add(var.set_pump_output(pump_output))

        if curr_soil_sensor := pump_config.get(CONF_CURR_SOIL):
            template_ = await cg.templatable(curr_soil_sensor, [], cg.int16)
            cg.add(var.set_curr_soil(template_))

        if curr_light_sensor := pump_config.get(CONF_CURR_LIGHT):
            template_ = await cg.templatable(curr_light_sensor, [], cg.int16)
            cg.add(var.set_curr_light(template_))

        if curr_temperature := pump_config.get(CONF_CURR_TEMPERATURE):
            template_ = await cg.templatable(curr_temperature, [], cg.int16)
            cg.add(var.set_curr_temperature(template_))

        min_temperature_number_var = await number.new_number(pump_config[CONF_MIN_TEMPERATURE], min_value=-50, max_value=50, step=1)
        await cg.register_component(min_temperature_number_var, pump_config[CONF_MIN_TEMPERATURE])
        cg.add(var.set_min_temperature_number(min_temperature_number_var))

        max_moisture_number_var = await number.new_number(pump_config[CONF_MAX_MOISTURE], min_value=0, max_value=100, step=1)
        await cg.register_component(max_moisture_number_var, pump_config[CONF_MAX_MOISTURE])
        cg.add(var.set_max_moisture_number(max_moisture_number_var))

        min_light_number_var = await number.new_number(pump_config[CONF_MIN_LIGHT], min_value=0, max_value=100, step=1)
        await cg.register_component(min_light_number_var, pump_config[CONF_MIN_LIGHT])
        cg.add(var.set_min_light_number(min_light_number_var))

        duration_number_var = await number.new_number(pump_config[CONF_RUN_DURATION], min_value=0, max_value=60*10, step=5)
        await cg.register_component(duration_number_var, pump_config[CONF_RUN_DURATION])
        cg.add(var.set_duration_number(duration_number_var))

        frequency_number_var = await number.new_number(pump_config[CONF_FREQUENCY], min_value=0, max_value=60*60*24, step=5)
        await cg.register_component(frequency_number_var, pump_config[CONF_FREQUENCY])
        cg.add(var.set_frequency_number(frequency_number_var))

        auto_mode_switch_var = await switch.new_switch(pump_config[CONF_AUTO_MODE])
        await cg.register_component(auto_mode_switch_var, pump_config[CONF_AUTO_MODE])
        cg.add(var.set_auto_mode_switch(auto_mode_switch_var))

        pump_running_sensor = await binary_sensor.new_binary_sensor(pump_config[CONF_PUMP_RUNNING])
        await cg.register_component(pump_running_sensor, pump_config[CONF_PUMP_RUNNING])
        cg.add(var.set_pump_running_binary_sensor(pump_running_sensor))
