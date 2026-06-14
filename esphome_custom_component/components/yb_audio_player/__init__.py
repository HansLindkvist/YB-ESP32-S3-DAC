import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, i2s_audio
from esphome.const import CONF_ID

AUTO_LOAD = ["i2c", "i2s_audio"]

yb_audio_ns = cg.esphome_ns.namespace("yb_audio")
YBAudioPlayer = yb_audio_ns.class_("YBAudioPlayer", cg.Component)

CONF_I2C_ID = "i2c_id"
CONF_I2S_ID = "i2s_id"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(YBAudioPlayer),
    cv.GenerateID(CONF_I2C_ID): cv.use_id(i2c.I2CComponent),
    cv.GenerateID(CONF_I2S_ID): cv.use_id(i2s_audio.I2SAudioComponent),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    i2c_component = await cg.get_variable(config[CONF_I2C_ID])
    cg.add(var.set_i2c_component(i2c_component))
    
    i2s_component = await cg.get_variable(config[CONF_I2S_ID])
    cg.add(var.set_i2s_audio_component(i2s_component))
