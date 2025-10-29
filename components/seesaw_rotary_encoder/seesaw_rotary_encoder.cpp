#include "seesaw_rotary_encoder.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace seesaw_rotary_encoder {

static const char *const TAG = "seesaw_rotary_encoder";

float SeesawRotaryEncoder::get_setup_priority() const { return setup_priority::IO; }

static const char *hw_id_to_string(uint8_t id) {
  switch (id) {
    case SEESAW_HW_ID_SAMD09:
      return "SAMD09";
    case SEESAW_HW_ID_TINY806:
      return "ATtiny806";
    case SEESAW_HW_ID_TINY807:
      return "ATtiny807";
    case SEESAW_HW_ID_TINY816:
      return "ATtiny816";
    case SEESAW_HW_ID_TINY817:
      return "ATtiny817";
    default:
      return "Unknown";
  }
}

void SeesawRotaryEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw Rotary Encoder...");

  // Read hardware ID
  uint8_t hw_id;
  if (this->readbuf(SEESAW_STATUS, SEESAW_STATUS_HW_ID, &hw_id, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to read hardware ID");
    this->mark_failed();
    return;
  }
  this->hw_id_ = hw_id;

  // Read version
  uint8_t version_buf[4];
  if (this->readbuf(SEESAW_STATUS, SEESAW_STATUS_VERSION, version_buf, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to read version");
    this->mark_failed();
    return;
  }
  this->version_ = (version_buf[0] << 24) | (version_buf[1] << 16) | (version_buf[2] << 8) | version_buf[3];
  this->product_id_ = (this->version_ >> 16) & 0xFFFF;

  // Verify this is the rotary encoder product
  if (this->product_id_ != SEESAW_PRODUCT_4991) {
    ESP_LOGW(TAG, "Product ID mismatch. Expected 4991, got %d", this->product_id_);
  }

  // Read options
  uint8_t options_buf[4];
  if (this->readbuf(SEESAW_STATUS, SEESAW_STATUS_OPTIONS, options_buf, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to read options");
    this->mark_failed();
    return;
  }
  this->options_ = (options_buf[0] << 24) | (options_buf[1] << 16) | (options_buf[2] << 8) | options_buf[3];

  ESP_LOGCONFIG(TAG, "Seesaw Rotary Encoder initialized successfully");
}

void SeesawRotaryEncoder::loop() {
  // Component loop - child components will handle their own updates
}

void SeesawRotaryEncoder::dump_config() {
  ESP_LOGCONFIG(TAG, "Seesaw Rotary Encoder:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Hardware ID: %s (0x%02X)", hw_id_to_string(this->hw_id_), this->hw_id_);
  ESP_LOGCONFIG(TAG, "  Product ID: %d", this->product_id_);
  uint32_t v = this->version_;
  ESP_LOGCONFIG(TAG, "  Version: %d.%d.%d", (v >> 16) & 0xFFFF, (v >> 8) & 0xFF, v & 0xFF);
  ESP_LOGCONFIG(TAG, "  Options: 0x%08X", this->options_);
}

// Encoder functions
void SeesawRotaryEncoder::enable_encoder_interrupt() {
  this->write8(SEESAW_ENCODER, SEESAW_ENCODER_INTENSET, 0x01);
}

int32_t SeesawRotaryEncoder::get_encoder_position() {
  uint8_t buf[4];
  if (this->readbuf(SEESAW_ENCODER, SEESAW_ENCODER_POSITION, buf, 4) != i2c::ERROR_OK) {
    return 0;
  }
  int32_t position = ((int32_t) buf[0] << 24) | ((int32_t) buf[1] << 16) | ((int32_t) buf[2] << 8) | (int32_t) buf[3];
  return -position;  // Invert to make clockwise positive
}

int32_t SeesawRotaryEncoder::get_encoder_delta() {
  uint8_t buf[4];
  if (this->readbuf(SEESAW_ENCODER, SEESAW_ENCODER_DELTA, buf, 4) != i2c::ERROR_OK) {
    return 0;
  }
  int32_t delta = ((int32_t) buf[0] << 24) | ((int32_t) buf[1] << 16) | ((int32_t) buf[2] << 8) | (int32_t) buf[3];
  return -delta;  // Invert to make clockwise positive
}

void SeesawRotaryEncoder::set_encoder_position(int32_t pos) {
  this->write32(SEESAW_ENCODER, SEESAW_ENCODER_POSITION, -pos);  // Invert back
}

// GPIO functions
void SeesawRotaryEncoder::set_pin_mode_pullup(uint8_t pin) {
  uint32_t pin_mask = 1UL << pin;
  // Set as input
  this->write32(SEESAW_GPIO, SEESAW_GPIO_DIRCLR_BULK, pin_mask);
  // Enable pullup
  this->write32(SEESAW_GPIO, SEESAW_GPIO_PULLENSET, pin_mask);
  // Set high for pullup
  this->write32(SEESAW_GPIO, SEESAW_GPIO_BULK_SET, pin_mask);
}

bool SeesawRotaryEncoder::digital_read(uint8_t pin) {
  uint8_t buf[4];
  if (this->readbuf(SEESAW_GPIO, SEESAW_GPIO_BULK, buf, 4) != i2c::ERROR_OK) {
    return false;
  }
  uint32_t gpio_state = ((uint32_t) buf[0] << 24) | ((uint32_t) buf[1] << 16) | ((uint32_t) buf[2] << 8) | (uint32_t) buf[3];
  return (gpio_state & (1UL << pin)) != 0;
}

void SeesawRotaryEncoder::set_gpio_interrupt(uint8_t pin, bool enabled) {
  uint32_t pin_mask = 1UL << pin;
  if (enabled) {
    this->write32(SEESAW_GPIO, SEESAW_GPIO_INTENSET, pin_mask);
  } else {
    this->write32(SEESAW_GPIO, SEESAW_GPIO_INTENCLR, pin_mask);
  }
}

// NeoPixel functions
void SeesawRotaryEncoder::setup_neopixel(uint8_t pin, uint16_t num_pixels) {
  this->neopixel_count_ = num_pixels;
  
  // Set NeoPixel speed (800 KHz = 1, 400 KHz = 0)
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_SPEED, 1);
  
  // Set buffer length (3 bytes per pixel for RGB)
  this->write16(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_BUF_LENGTH, num_pixels * 3);
  
  // Set NeoPixel pin
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_PIN, pin);
}

void SeesawRotaryEncoder::set_neopixel_color(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b) {
  if (pixel >= this->neopixel_count_) {
    return;
  }

  // Apply brightness scaling
  r = (r * this->neopixel_brightness_) / 255;
  g = (g * this->neopixel_brightness_) / 255;
  b = (b * this->neopixel_brightness_) / 255;

  // NeoPixel buffer format: module, register, offset (2 bytes), G, R, B
  uint8_t buf[7];
  buf[0] = SEESAW_NEOPIXEL;
  buf[1] = SEESAW_NEOPIXEL_BUF;
  buf[2] = (pixel * 3) >> 8;    // Offset high byte
  buf[3] = (pixel * 3) & 0xFF;  // Offset low byte
  buf[4] = g;  // NeoPixels use GRB order
  buf[5] = r;
  buf[6] = b;

  this->write(buf, 7);
}

void SeesawRotaryEncoder::set_neopixel_brightness(uint8_t brightness) {
  this->neopixel_brightness_ = brightness;
}

void SeesawRotaryEncoder::show_neopixel() {
  uint8_t buf[2] = {SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_SHOW};
  this->write(buf, 2);
}

// I2C helper functions
i2c::ErrorCode SeesawRotaryEncoder::write8(SeesawModule mod, uint8_t reg, uint8_t value) {
  uint8_t buf[3] = {mod, reg, value};
  return this->write(buf, 3);
}

i2c::ErrorCode SeesawRotaryEncoder::write16(SeesawModule mod, uint8_t reg, uint16_t value) {
  uint8_t buf[4] = {mod, reg, (uint8_t) (value >> 8), (uint8_t) (value & 0xFF)};
  return this->write(buf, 4);
}

i2c::ErrorCode SeesawRotaryEncoder::write32(SeesawModule mod, uint8_t reg, uint32_t value) {
  uint8_t buf[6] = {mod, reg, (uint8_t) (value >> 24), (uint8_t) (value >> 16),
                    (uint8_t) (value >> 8), (uint8_t) (value & 0xFF)};
  return this->write(buf, 6);
}

i2c::ErrorCode SeesawRotaryEncoder::readbuf(SeesawModule mod, uint8_t reg, uint8_t *buf, uint8_t len) {
  uint8_t cmdbuf[2] = {mod, reg};
  i2c::ErrorCode err = this->write(cmdbuf, 2);
  if (err != i2c::ERROR_OK) {
    return err;
  }
  delay(1);  // Small delay for seesaw to process
  return this->read(buf, len);
}

i2c::ErrorCode SeesawRotaryEncoder::writebuf(SeesawModule mod, uint8_t reg, const uint8_t *buf, uint8_t len) {
  uint8_t *cmdbuf = new uint8_t[len + 2];
  cmdbuf[0] = mod;
  cmdbuf[1] = reg;
  memcpy(cmdbuf + 2, buf, len);
  i2c::ErrorCode err = this->write(cmdbuf, len + 2);
  delete[] cmdbuf;
  return err;
}

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
