#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "../seesaw_rotary_encoder.h"

namespace esphome {
namespace seesaw_rotary_encoder {

class SeesawRotaryEncoderLight : public light::LightOutput, public Component {
 public:
  void setup() override;
  void dump_config() override;
  
  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;

  void set_parent(SeesawRotaryEncoder *parent) { this->parent_ = parent; }

 protected:
  SeesawRotaryEncoder *parent_;
};

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
