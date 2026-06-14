#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/i2s_audio/i2s_audio.h"
#include "Arduino.h"

namespace esphome {
namespace yb_audio {

class YBAudioPlayer : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  
  void set_i2c_component(i2c::I2CComponent *i2c) { this->i2c_ = i2c; }
  void set_i2s_audio_component(i2s_audio::I2SAudioComponent *i2s) { this->i2s_audio_ = i2s; }
  
  // Public API methods for Home Assistant
  void play_tone(float frequency, uint16_t duration_ms);
  void play_test_sound();
  void stop_playback();
  bool is_playing() { return this->playing_; }
  
 private:
  i2c::I2CComponent *i2c_{nullptr};
  i2s_audio::I2SAudioComponent *i2s_audio_{nullptr};
  
  // TLV320DAC3101 I2C address
  static constexpr uint8_t TLV320_ADDR = 0x18;
  
  // TLV320DAC3101 Register addresses
  static constexpr uint8_t REG_PAGE_SELECT = 0x00;
  static constexpr uint8_t REG_RESET = 0x01;
  static constexpr uint8_t REG_POWER_CONTROL = 0x02;
  static constexpr uint8_t REG_CODEC_DATAPATH = 0x07;
  static constexpr uint8_t REG_SERIAL_IF_CTRL_A = 0x1B;
  static constexpr uint8_t REG_SERIAL_IF_CTRL_B = 0x1C;
  static constexpr uint8_t REG_SERIAL_IF_CTRL_C = 0x1D;
  static constexpr uint8_t REG_LEFT_DAC_VOL = 0x43;
  static constexpr uint8_t REG_RIGHT_DAC_VOL = 0x44;
  
  bool playing_{false};
  uint32_t sound_end_time_{0};
  
  void init_tlv320_dac();
  void write_register(uint8_t reg, uint8_t value);
  uint8_t read_register(uint8_t reg);
  void set_volume(uint8_t volume);
};

}  // namespace yb_audio
}  // namespace esphome
