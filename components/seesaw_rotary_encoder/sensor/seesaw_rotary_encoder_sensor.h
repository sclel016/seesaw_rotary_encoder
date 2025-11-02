#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../seesaw_rotary_encoder.h"

namespace esphome {
namespace seesaw_rotary_encoder {

class SeesawRotaryEncoderSensor : public sensor::Sensor, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  // Setters needed by ESPHome code generation
  void set_parent(SeesawRotaryEncoder *parent) { this->parent_ = parent; }
  void set_min_value(int32_t min_value) { this->min_value_ = min_value; }
  void set_max_value(int32_t max_value) { this->max_value_ = max_value; }
  void set_step(float step) { this->step_ = step; }
  
  // Public method to set encoder position (for use in automations)
  void set_position(int32_t value) {
    value = std::max(std::min(value, this->max_value_), this->min_value_);
    this->last_position_ = value;
    this->parent_->set_encoder_position(value);
    this->publish_state(value);
  }
  
  // Public member variables for direct access from YAML
  int32_t min_value_{0};
  int32_t max_value_{100};
  float step_{1.0f};

 protected:
  SeesawRotaryEncoder *parent_;
  
  // Conditioned position value (clamped to min/max, scaled by step)
  // This is what gets reported to Home Assistant
  int32_t last_position_{0};
  
  // Timestamp of last update for polling control
  uint32_t last_update_ms_{0};
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
