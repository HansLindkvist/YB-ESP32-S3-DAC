# ESPHome Custom Component for YB-ESP32-S3-DAC

This custom component enables the **YB-ESP32-S3-DAC** audio development board to work with **ESPHome** running under **Home Assistant**.

## Features

- Play arbitrary recognizable sounds on the headphone/earphone connector
- Control audio playback from Home Assistant
- Full TLV320DAC3101 DAC initialization and configuration
- I2C control interface for the TLV320 DAC
- I2S audio streaming support
- Volume control
- Integration with ESPHome's logging and configuration system

## Component Structure

```
esphome_custom_component/
└── components/
    └── yb_audio_player/
        ├── __init__.py          # ESPHome component configuration
        ├── manifest.json        # Component metadata
        ├── yb_audio_player.h    # C++ header file
        └── yb_audio_player.cpp  # C++ implementation
```

## Installation

### Quick Start: Using the GitHub Reference

The easiest way to add this component to your ESPHome device is to reference it directly from GitHub:

1. **Open ESPHome Dashboard** in Home Assistant (Settings → Devices & Services → ESPHome)
2. **Click EDIT** on your YB-ESP32-S3-DAC device
3. **Add the external component reference** to your device's YAML:

```yaml
external_components:
  - source: github://HansLindkvist/YB-ESP32-S3-DAC/esphome_custom_component/components
    components: [yb_audio_player]
```

4. **Click SAVE** and then **INSTALL** to compile and upload

### Alternative: Local Installation

If you prefer to work with the component locally:

1. **Clone the repository:**
   ```bash
   cd /config/esphome/  # Home Assistant config directory
   git clone https://github.com/HansLindkvist/YB-ESP32-S3-DAC.git
   ```

2. **Copy the component:**
   ```bash
   cp -r YB-ESP32-S3-DAC/esphome_custom_component ./custom_components/
   ```

3. **Reference it locally in your YAML:**
   ```yaml
   external_components:
     - source: local
       components: [yb_audio_player]
   ```

## Basic Configuration

Add the following to your ESPHome device configuration:

```yaml
esphome:
  name: yb-esp32-s3-dac-audio

esp32:
  board: esp32s3box
  framework:
    type: arduino

# I2C configuration (for DAC control)
i2c:
  id: i2c_dac
  sda: GPIO8
  scl: GPIO9
  frequency: 100kHz

# I2S Audio configuration (for audio streaming)
i2s_audio:
  id: i2s_dac
  i2s_lrclk_pin: GPIO6
  i2s_bclk_pin: GPIO5
  i2s_dout_pin: GPIO7
  i2s_mclk_pin: GPIO4
  bits_per_sample: 16

# External component reference - note the /components at the end
external_components:
  - source: github://HansLindkvist/YB-ESP32-S3-DAC/esphome_custom_component/components
    components: [yb_audio_player]

# YB Audio Player component
yb_audio_player:
  i2c_id: i2c_dac
  i2s_id: i2s_dac
```

See `esphome_example_config.yaml` for a complete configuration example.

## GPIO Pin Mapping

The component uses the following GPIO pins on the YB-ESP32-S3-DAC:

| Function | GPIO | Purpose |
|----------|------|----------|
| I2C SDA  | GPIO8 | DAC Control |
| I2C SCL  | GPIO9 | DAC Control |
| I2S LRCLK | GPIO6 | Audio Frame Clock |
| I2S BCLK | GPIO5 | Audio Bit Clock |
| I2S DIN | GPIO7 | Audio Data Input |
| I2S MCLK | GPIO4 | Master Clock (optional) |

## Usage Examples

### Complete Example Configuration

Refer to `esphome_example_config.yaml` for a fully working configuration including:
- WiFi setup with fallback AP
- Home Assistant API integration with encryption
- Logging configuration
- I2C and I2S interfaces
- YB Audio Player component
- Example button and switch entities

### Adding a Button to Play Sound

```yaml
button:
  - platform: template
    name: "Play Alert Sound"
    on_press:
      then:
        - logger.log: "Playing alert sound"
```

### Adding a Switch to Control Audio

```yaml
switch:
  - platform: template
    name: "Audio Output"
    turn_on_action:
      - logger.log: "Audio enabled"
    turn_off_action:
      - logger.log: "Audio disabled"
```

## API Methods (C++)

The component exposes the following methods for use in automations and templates:

- `play_tone(float frequency, uint16_t duration_ms)` - Play a tone at specified frequency
- `play_test_sound()` - Play a test beep pattern (3 short beeps at 1kHz)
- `stop_playback()` - Stop current playback
- `is_playing()` - Check if audio is currently playing

## TLV320DAC3101 Configuration

The component automatically initializes the TLV320DAC3101 DAC with:

- I2C address: 0x18
- I2S mode enabled
- Default volume level set to mid-range
- Headphone output enabled

For more details on the DAC configuration, refer to the [TLV320DAC3101 datasheet](https://www.ti.com/product/TLV320DAC3101).

## Secrets Configuration

The example configuration uses the following secrets (add to `secrets.yaml` in your ESPHome directory):

```yaml
wifi_ssid: "Your WiFi Network Name"
wifi_password: "Your WiFi Password"
wifi_ap_password: "Your Fallback AP Password"
```

## Building and Flashing

### Via ESPHome Dashboard

1. Edit your device configuration
2. Click the **INSTALL** button
3. Select your connection method (USB, Wireless, etc.)
4. ESPHome will compile and flash your device

### Via Command Line

```bash
# Compile
esphome compile esphome_example_config.yaml

# Upload to device
esphome upload esphome_example_config.yaml

# View logs
esphome logs esphome_example_config.yaml
```

## Troubleshooting

### Component Import Errors

If you get an error like `Could not find components folder for source`, make sure:
- The source path includes `/components` at the end: `github://HansLindkvist/YB-ESP32-S3-DAC/esphome_custom_component/components`
- Your GitHub repository has the correct folder structure: `esphome_custom_component/components/yb_audio_player/`
- You're using `external_components` (not `externally_defined_custom_components`)

### I2C Communication Issues

- Ensure GPIO8 (SDA) and GPIO9 (SCL) are properly configured
- Check that I2C frequency is set to 100kHz
- Verify the TLV320 I2C address (0x18) is correct
- Look for I2C error messages in the ESPHome logs

### No Audio Output

- Verify the I2S pins are correctly connected (GPIO4, GPIO5, GPIO6, GPIO7)
- Check DAC volume settings (registers 0x43, 0x44)
- Ensure the headphone/earphone connector is properly connected
- Verify audio data is being streamed via I2S
- Check ESPHome logs for any initialization errors

### Component Initialization Failures

- Check ESPHome logs for initialization errors
- Ensure both I2C and I2S components are properly configured and their IDs are referenced correctly
- Verify the board has sufficient power (5V/1A recommended for audio output)
- Make sure the GPIO pins are not used elsewhere in your configuration

### OTA Updates

OTA (Over-The-Air) updates are enabled by default with a password. If you want to change the OTA password, update your configuration:

```yaml
ota:
  - platform: esphome
    password: "your_new_ota_password"
```

## References

- [YB-ESP32-S3-DAC GitHub Repository](https://github.com/HansLindkvist/YB-ESP32-S3-DAC)
- [ESPHome Documentation](https://esphome.io/)
- [ESPHome External Components](https://esphome.io/components/external_components.html)
- [TLV320DAC3101 Datasheet](https://www.ti.com/product/TLV320DAC3101)
- [ESP32-S3 I2S Audio Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/i2s.html)
- [Home Assistant ESPHome Integration](https://www.home-assistant.io/integrations/esphome/)

## License

This component is provided as-is for use with the YB-ESP32-S3-DAC board.
