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
  
  // Public member variables for direct access from YAML
  int32_t min_value_{INT32_MIN};
  int32_t max_value_{INT32_MAX};
  float step_{1.0f};

 protected:
  SeesawRotaryEncoder *parent_;
  
  // Conditioned position value (clamped to min/max, scaled by step)
  // This is what gets reported to Home Assistant
  int32_t last_position_{0};
  
  // Raw absolute position from hardware encoder (unbounded)
  int32_t last_raw_position_{0};
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
