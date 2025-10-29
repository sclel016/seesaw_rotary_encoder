#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../seesaw_rotary_encoder.h"

namespace esphome {
namespace seesaw_rotary_encoder {

class SeesawRotaryEncoderButton : public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_parent(SeesawRotaryEncoder *parent) { this->parent_ = parent; }

 protected:
  SeesawRotaryEncoder *parent_;
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
