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
└── yb_audio_player/
    ├── __init__.py          # ESPHome component configuration
    ├── manifest.json        # Component metadata
    ├── yb_audio_player.h    # C++ header file
    └── yb_audio_player.cpp  # C++ implementation
```

## Installation

### 1. Prepare Your ESPHome Configuration

Add the custom component to your ESPHome `configuration.yaml`:

```yaml
externally_defined_custom_components:
  - source: github://HansLindkvist/YB-ESP32-S3-DAC/esphome_custom_component
    components: [yb_audio_player]
```

### 2. Configure I2C and I2S Interfaces

The component requires both I2C (for DAC control) and I2S (for audio streaming):

```yaml
i2c:
  id: i2c_dac
  sda: GPIO8
  scl: GPIO9
  frequency: 100kHz

i2s_audio:
  id: i2s_dac
  i2s_lrclk_pin: GPIO6
  i2s_bclk_pin: GPIO5
  i2s_dout_pin: GPIO7
  i2s_mclk_pin: GPIO4
  bits_per_sample: 16
```

### 3. Add the YB Audio Player Component

```yaml
yb_audio_player:
  i2c_id: i2c_dac
  i2s_id: i2s_dac
```

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

### Example 1: Basic Configuration

See `esphome_example_config.yaml` for a complete working example.

### Example 2: Add a Button to Play Sound

```yaml
button:
  - platform: template
    name: "Play Alert Sound"
    on_press:
      then:
        - logger.log: "Playing alert sound"
```

### Example 3: Add a Switch to Control Audio

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

The component exposes the following methods:

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

## Development Notes

### Building the Component

To compile the component for your ESP32-S3:

```bash
esphome compile esphome_example_config.yaml
```

### Flashing to the Board

```bash
esphome upload esphome_example_config.yaml
```

### Monitoring Logs

```bash
esphome logs esphome_example_config.yaml
```

## Troubleshooting

### I2C Communication Issues

- Ensure GPIO8 (SDA) and GPIO9 (SCL) are properly configured
- Check that I2C frequency is set to 100kHz
- Verify the TLV320 I2C address (0x18) is correct

### No Audio Output

- Verify the I2S pins are correctly connected (GPIO4, GPIO5, GPIO6, GPIO7)
- Check DAC volume settings (registers 0x43, 0x44)
- Ensure the headphone connector is properly connected
- Verify audio data is being streamed via I2S

### Component Initialization Failures

- Check ESPHome logs for initialization errors
- Ensure both I2C and I2S components are properly configured
- Verify the board has sufficient power

## References

- [YB-ESP32-S3-DAC GitHub Repository](https://github.com/yellobyte/YB-ESP32-S3-DAC)
- [ESPHome Documentation](https://esphome.io/)
- [TLV320DAC3101 Datasheet](https://www.ti.com/product/TLV320DAC3101)
- [ESP32-S3 I2S Audio Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/i2s.html)

## License

This component is provided as-is for use with the YB-ESP32-S3-DAC board.
