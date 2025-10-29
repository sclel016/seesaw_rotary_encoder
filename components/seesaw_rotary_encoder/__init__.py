import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]
MULTI_CONF = True

seesaw_rotary_encoder_ns = cg.esphome_ns.namespace("seesaw_rotary_encoder")
SeesawRotaryEncoder = seesaw_rotary_encoder_ns.class_(
    "SeesawRotaryEncoder", i2c.I2CDevice, cg.Component
)

CONF_SEESAW_ROTARY_ENCODER = "seesaw_rotary_encoder"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SeesawRotaryEncoder),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
