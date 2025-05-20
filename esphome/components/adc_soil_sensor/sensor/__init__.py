import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import output, switch, number, binary_sensor
from esphome.components.adc import sensor as adc

from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
    CONF_NAME,
    CONF_THRESHOLD,
    CONF_SENSOR_ID,
)

AUTO_LOAD = ["number", "switch", "binary_sensor", "output", "button"]
CODEOWNERS = ["@manoamaro"]
IS_PLATFORM_COMPONENT = True

CONF_ENABLED_SWITCH = "enabled_switch"
CONF_CALLIBRATION_DRY_BUTTON = "calibration_dry_button"
CONF_CALLIBRATION_WET_BUTTON = "calibration_wet_button"

adc_soil_sensor_ns = cg.esphome_ns.namespace("adc_soil_sensor")
AdcSoilSensor = adc_soil_sensor_ns.class_("AdcSoilSensor", adc.ADCSensor)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(AdcSoilSensor),
        cv.Required(CONF_SENSOR_ID): cv.use_id(adc.ADCSensor),
        cv.Optional(CONF_ENABLED_SWITCH, {CONF_NAME: "Enabled"}): switch.switch_schema(
            switch.Switch,
            default_restore_mode="RESTORE_DEFAULT_OFF",
        ),
        cv.Optional(CONF_THRESHOLD, default=0.5): cv.templatable(cv.percentage),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
