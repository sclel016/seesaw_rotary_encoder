import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    STATE_CLASS_NONE,
    UNIT_STEPS,
    ICON_ROTATE_RIGHT,
)

CONF_STEP = "step"
from .. import (
    seesaw_rotary_encoder_ns,
    SeesawRotaryEncoder,
    CONF_SEESAW_ROTARY_ENCODER,
)

SeesawRotaryEncoderSensor = seesaw_rotary_encoder_ns.class_(
    "SeesawRotaryEncoderSensor", sensor.Sensor, cg.Component
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        SeesawRotaryEncoderSensor,
        unit_of_measurement=UNIT_STEPS,
        icon=ICON_ROTATE_RIGHT,
        accuracy_decimals=0,
        state_class=STATE_CLASS_NONE,
    )
    .extend(
        {
            cv.GenerateID(CONF_SEESAW_ROTARY_ENCODER): cv.use_id(SeesawRotaryEncoder),
            cv.Optional(CONF_MIN_VALUE): cv.int_,
            cv.Optional(CONF_MAX_VALUE): cv.int_,
            cv.Optional(CONF_STEP, default=1): cv.positive_float,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    
    parent = await cg.get_variable(config[CONF_SEESAW_ROTARY_ENCODER])
    cg.add(var.set_parent(parent))
    
    if CONF_MIN_VALUE in config:
        cg.add(var.set_min_value(config[CONF_MIN_VALUE]))
    if CONF_MAX_VALUE in config:
        cg.add(var.set_max_value(config[CONF_MAX_VALUE]))
    cg.add(var.set_step(config[CONF_STEP]))
