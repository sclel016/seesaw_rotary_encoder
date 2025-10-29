***WARNING: Written entirely by Claude - use at your own peril...***

# Seesaw Rotary Encoder Component for ESPHome

This is a custom ESPHome component for the Adafruit I2C QT Rotary Encoder (Product ID 4991). It provides full support for:

- **Rotary Encoder** - Track rotation position with configurable min/max values
- **Push Button** - Built-in switch as a binary sensor
- **NeoPixel LED** - RGB LED control with brightness

## Hardware

- [Adafruit I2C QT Rotary Encoder](https://www.adafruit.com/product/4991)
- Default I2C Address: `0x36`
- Button Pin: 24 (SS_SWITCH)
- NeoPixel Pin: 6 (SS_NEOPIX)

## Installation

Add this component to your ESPHome configuration by adding it as an external component:

```yaml
external_components:
  - source:
      type: local
      path: /path/to/seesaw_rotary_encoder
```

Or if using from a git repository:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/sclel016/seesaw_rotary_encoder
```

## Configuration

### Basic Configuration

```yaml
# I2C bus configuration (if not already configured)
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

# Main component
seesaw_rotary_encoder:
  id: my_encoder
  address: 0x36  # Default address

# Rotary encoder sensor
sensor:
  - platform: seesaw_rotary_encoder
    seesaw_rotary_encoder_id: my_encoder
    name: "Rotary Encoder Position"
    id: encoder_position
    # Optional: Set min/max values
    # min_value: 0
    # max_value: 100
    # step: 1

# Push button
binary_sensor:
  - platform: seesaw_rotary_encoder
    seesaw_rotary_encoder_id: my_encoder
    name: "Rotary Encoder Button"
    id: encoder_button
    on_press:
      then:
        - logger.log: "Button pressed!"

# NeoPixel LED
light:
  - platform: seesaw_rotary_encoder
    seesaw_rotary_encoder_id: my_encoder
    name: "Rotary Encoder LED"
    id: encoder_led
```


## Credits

Vibe coded by Claude:
- Adafruit_Seesaw library
- ESPHome seesaw component by @ssieb
- ESPHome framework

## License

MIT License - Feel free to use and modify
