#include "seesaw_rotary_encoder_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw_rotary_encoder {

static const char *const TAG = "seesaw_rotary_encoder.binary_sensor";

void SeesawRotaryEncoderButton::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw Rotary Encoder Button...");
  
  // Configure switch pin with pullup
  this->parent_->set_pin_mode_pullup(SS_SWITCH);
  
  // Enable GPIO interrupt for the switch
  this->parent_->set_gpio_interrupt(SS_SWITCH, true);
  
  // Read initial state (inverted because of pullup - pressed = LOW)
  bool state = !this->parent_->digital_read(SS_SWITCH);
  this->publish_initial_state(state);
}

void SeesawRotaryEncoderButton::loop() {
  // Read button state (inverted because of pullup - pressed = LOW)
  bool state = !this->parent_->digital_read(SS_SWITCH);
  this->publish_state(state);
}

void SeesawRotaryEncoderButton::dump_config() {
  ESP_LOGCONFIG(TAG, "Seesaw Rotary Encoder Button:");
  LOG_BINARY_SENSOR("  ", "Button", this);
  ESP_LOGCONFIG(TAG, "  Pin: %d", SS_SWITCH);
}

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
