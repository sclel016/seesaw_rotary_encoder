import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from .. import (
    seesaw_rotary_encoder_ns,
    SeesawRotaryEncoder,
    CONF_SEESAW_ROTARY_ENCODER,
)

SeesawRotaryEncoderButton = seesaw_rotary_encoder_ns.class_(
    "SeesawRotaryEncoderButton", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(SeesawRotaryEncoderButton)
    .extend(
        {
            cv.GenerateID(CONF_SEESAW_ROTARY_ENCODER): cv.use_id(SeesawRotaryEncoder),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
    
    parent = await cg.get_variable(config[CONF_SEESAW_ROTARY_ENCODER])
    cg.add(var.set_parent(parent))
