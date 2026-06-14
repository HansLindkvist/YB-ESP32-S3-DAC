#include "yb_audio_player.h"
#include "esphome/core/log.h"

namespace esphome {
namespace yb_audio {

static const char *const TAG = "yb_audio_player";

void YBAudioPlayer::setup() {
  ESP_LOGI(TAG, "Initializing YB-ESP32-S3-DAC Audio Player...");
  
  if (!this->i2c_) {
    ESP_LOGE(TAG, "I2C component not set!");
    this->mark_failed();
    return;
  }
  
  if (!this->i2s_audio_) {
    ESP_LOGE(TAG, "I2S Audio component not set!");
    this->mark_failed();
    return;
  }
  
  // Initialize TLV320DAC3101
  this->init_tlv320_dac();
  ESP_LOGI(TAG, "YB-ESP32-S3-DAC Audio Player initialized successfully");
}

void YBAudioPlayer::loop() {
  if (this->playing_ && millis() >= this->sound_end_time_) {
    this->playing_ = false;
    ESP_LOGI(TAG, "Playback finished");
  }
}

void YBAudioPlayer::dump_config() {
  ESP_LOGCONFIG(TAG, "YB-ESP32-S3-DAC Audio Player:");
  ESP_LOGCONFIG(TAG, "  I2C Address: 0x%02X", TLV320_ADDR);
  ESP_LOGCONFIG(TAG, "  I2C Bus initialized: %s", this->i2c_ ? "YES" : "NO");
  ESP_LOGCONFIG(TAG, "  I2S Audio initialized: %s", this->i2s_audio_ ? "YES" : "NO");
}

void YBAudioPlayer::init_tlv320_dac() {
  ESP_LOGI(TAG, "Initializing TLV320DAC3101...");
  
  // Reset the DAC
  this->write_register(REG_RESET, 0x01);
  delay(10);
  
  // Power up the DAC
  // Page 0 register 2: Power control
  // Set bits to power up DAC and analog outputs
  this->write_register(REG_POWER_CONTROL, 0xD4);  // Power up recommended values
  delay(10);
  
  // Configure codec datapath (Page 0, Register 7)
  // Set up for I2S input, DAC output
  this->write_register(REG_CODEC_DATAPATH, 0x0A);  // Recommended settings
  
  // Configure Serial Interface Control A (Page 0, Register 27)
  this->write_register(REG_SERIAL_IF_CTRL_A, 0x00);  // Default I2S mode
  
  // Configure Serial Interface Control B (Page 0, Register 28)
  this->write_register(REG_SERIAL_IF_CTRL_B, 0x00);  // 16-bit, Left-justified
  
  // Configure Serial Interface Control C (Page 0, Register 29)
  this->write_register(REG_SERIAL_IF_CTRL_C, 0x00);  // Slave mode
  
  // Set DAC volume to a reasonable level (Page 0, Registers 67 & 68)
  this->set_volume(0xA0);  // Mid-range volume
  
  ESP_LOGI(TAG, "TLV320DAC3101 initialization complete");
}

void YBAudioPlayer::write_register(uint8_t reg, uint8_t value) {
  if (!this->i2c_) return;
  
  uint8_t data[2] = {reg, value};
  auto err = this->i2c_->write(TLV320_ADDR, data, 2);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write register 0x%02X: error %d", reg, err);
  }
}

uint8_t YBAudioPlayer::read_register(uint8_t reg) {
  if (!this->i2c_) return 0;
  
  uint8_t value = 0;
  auto err = this->i2c_->read(TLV320_ADDR, &reg, 1, &value, 1);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read register 0x%02X: error %d", reg, err);
  }
  return value;
}

void YBAudioPlayer::set_volume(uint8_t volume) {
  // Set left DAC volume (Page 0, Register 67)
  this->write_register(REG_LEFT_DAC_VOL, volume);
  // Set right DAC volume (Page 0, Register 68)
  this->write_register(REG_RIGHT_DAC_VOL, volume);
  ESP_LOGI(TAG, "Volume set to 0x%02X", volume);
}

void YBAudioPlayer::play_tone(float frequency, uint16_t duration_ms) {
  if (this->playing_) {
    ESP_LOGW(TAG, "Already playing, ignoring new tone request");
    return;
  }
  
  ESP_LOGI(TAG, "Playing tone: frequency=%.1f Hz, duration=%d ms", frequency, duration_ms);
  // Note: Actual tone generation requires I2S audio implementation
  // This is a placeholder for the logic
  
  this->playing_ = true;
  this->sound_end_time_ = millis() + duration_ms;
}

void YBAudioPlayer::play_test_sound() {
  // Play a simple beep pattern (3 short beeps)
  for (int i = 0; i < 3; i++) {
    this->play_tone(1000, 100);  // 1kHz for 100ms
    delay(150);
  }
  ESP_LOGI(TAG, "Test sound playback completed");
}

void YBAudioPlayer::stop_playback() {
  this->playing_ = false;
  this->sound_end_time_ = 0;
  ESP_LOGI(TAG, "Playback stopped");
}

}  // namespace yb_audio
}  // namespace esphome
