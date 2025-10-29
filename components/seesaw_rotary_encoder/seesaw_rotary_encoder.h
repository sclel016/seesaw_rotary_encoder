#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace seesaw_rotary_encoder {

// Seesaw module base addresses
enum SeesawModule : uint8_t {
  SEESAW_STATUS = 0x00,
  SEESAW_GPIO = 0x01,
  SEESAW_NEOPIXEL = 0x0E,
  SEESAW_ENCODER = 0x11,
};

// Status module registers
enum : uint8_t {
  SEESAW_STATUS_HW_ID = 0x01,
  SEESAW_STATUS_VERSION = 0x02,
  SEESAW_STATUS_OPTIONS = 0x03,
  SEESAW_STATUS_SWRST = 0x7F,
};

// Encoder module registers
enum : uint8_t {
  SEESAW_ENCODER_STATUS = 0x00,
  SEESAW_ENCODER_INTENSET = 0x10,
  SEESAW_ENCODER_INTENCLR = 0x20,
  SEESAW_ENCODER_POSITION = 0x30,
  SEESAW_ENCODER_DELTA = 0x40,
};

// GPIO module registers
enum : uint8_t {
  SEESAW_GPIO_DIRSET_BULK = 0x02,
  SEESAW_GPIO_DIRCLR_BULK = 0x03,
  SEESAW_GPIO_BULK = 0x04,
  SEESAW_GPIO_BULK_SET = 0x05,
  SEESAW_GPIO_BULK_CLR = 0x06,
  SEESAW_GPIO_INTENSET = 0x08,
  SEESAW_GPIO_INTENCLR = 0x09,
  SEESAW_GPIO_PULLENSET = 0x0B,
};

// NeoPixel module registers
enum : uint8_t {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

// Hardware IDs
#define SEESAW_HW_ID_SAMD09 0x55
#define SEESAW_HW_ID_TINY806 0x84
#define SEESAW_HW_ID_TINY807 0x85
#define SEESAW_HW_ID_TINY816 0x86
#define SEESAW_HW_ID_TINY817 0x87

// Product ID for rotary encoder
#define SEESAW_PRODUCT_4991 4991

// Pin definitions for I2C QT Rotary Encoder
#define SS_SWITCH 24
#define SS_NEOPIX 6

class SeesawRotaryEncoder : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override;

  // Encoder functions
  void enable_encoder_interrupt();
  int32_t get_encoder_position();
  int32_t get_encoder_delta();
  void set_encoder_position(int32_t pos);

  // GPIO functions
  void set_pin_mode_pullup(uint8_t pin);
  bool digital_read(uint8_t pin);
  void set_gpio_interrupt(uint8_t pin, bool enabled);

  // NeoPixel functions
  void setup_neopixel(uint8_t pin, uint16_t num_pixels);
  void set_neopixel_color(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b);
  void set_neopixel_brightness(uint8_t brightness);
  void show_neopixel();

 protected:
  i2c::ErrorCode write8(SeesawModule mod, uint8_t reg, uint8_t value);
  i2c::ErrorCode write16(SeesawModule mod, uint8_t reg, uint16_t value);
  i2c::ErrorCode write32(SeesawModule mod, uint8_t reg, uint32_t value);
  i2c::ErrorCode readbuf(SeesawModule mod, uint8_t reg, uint8_t *buf, uint8_t len);
  i2c::ErrorCode writebuf(SeesawModule mod, uint8_t reg, const uint8_t *buf, uint8_t len);

  uint8_t hw_id_{0};
  uint32_t version_{0};
  uint32_t options_{0};
  uint16_t product_id_{0};
  uint8_t neopixel_brightness_{255};
  uint16_t neopixel_count_{0};
  uint8_t neopixel_buf_[9];  // Buffer for NeoPixel data
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
