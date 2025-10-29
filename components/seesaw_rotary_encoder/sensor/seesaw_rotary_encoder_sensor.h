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

  void set_parent(SeesawRotaryEncoder *parent) { this->parent_ = parent; }
  void set_min_value(int32_t min_value) { this->min_value_ = min_value; }
  void set_max_value(int32_t max_value) { this->max_value_ = max_value; }
  void set_step(int32_t step) { this->step_ = step; }

 protected:
  SeesawRotaryEncoder *parent_;
  int32_t last_position_{0};
  int32_t last_raw_position_{0};
  int32_t min_value_{INT32_MIN};
  int32_t max_value_{INT32_MAX};
  int32_t step_{1};
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
