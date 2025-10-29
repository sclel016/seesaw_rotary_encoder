#include "seesaw_rotary_encoder_light.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw_rotary_encoder {

static const char *const TAG = "seesaw_rotary_encoder.light";

void SeesawRotaryEncoderLight::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw Rotary Encoder NeoPixel...");
  
  // Initialize NeoPixel on pin 6 with 1 pixel
  this->parent_->setup_neopixel(SS_NEOPIX, 1);
  
  // Set initial brightness to 20 (like in the example)
  this->parent_->set_neopixel_brightness(20);
}

void SeesawRotaryEncoderLight::dump_config() {
  ESP_LOGCONFIG(TAG, "Seesaw Rotary Encoder NeoPixel:");
  ESP_LOGCONFIG(TAG, "  Pin: %d", SS_NEOPIX);
}

light::LightTraits SeesawRotaryEncoderLight::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

void SeesawRotaryEncoderLight::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  
  // Convert to 0-255 range
  uint8_t r = (uint8_t) (red * 255.0f);
  uint8_t g = (uint8_t) (green * 255.0f);
  uint8_t b = (uint8_t) (blue * 255.0f);
  
  // Set the color for pixel 0
  this->parent_->set_neopixel_color(0, r, g, b);
  
  // Show the update
  this->parent_->show_neopixel();
}

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
