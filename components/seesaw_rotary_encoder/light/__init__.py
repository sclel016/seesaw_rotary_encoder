import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID
from .. import (
    seesaw_rotary_encoder_ns,
    SeesawRotaryEncoder,
    CONF_SEESAW_ROTARY_ENCODER,
)

SeesawRotaryEncoderLight = seesaw_rotary_encoder_ns.class_(
    "SeesawRotaryEncoderLight", light.LightOutput, cg.Component
)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(SeesawRotaryEncoderLight),
        cv.GenerateID(CONF_SEESAW_ROTARY_ENCODER): cv.use_id(SeesawRotaryEncoder),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    
    parent = await cg.get_variable(config[CONF_SEESAW_ROTARY_ENCODER])
    cg.add(var.set_parent(parent))
