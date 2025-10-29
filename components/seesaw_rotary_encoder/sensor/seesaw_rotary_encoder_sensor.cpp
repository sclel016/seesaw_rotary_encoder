#include "seesaw_rotary_encoder_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw_rotary_encoder {

static const char *const TAG = "seesaw_rotary_encoder.sensor";

void SeesawRotaryEncoderSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw Rotary Encoder Sensor...");
  
  // Enable encoder interrupt
  this->parent_->enable_encoder_interrupt();
  
  // Get initial position
  this->last_position_ = this->parent_->get_encoder_position();
  
  // Clamp to min/max if set
  if (this->last_position_ < this->min_value_) {
    this->last_position_ = this->min_value_;
    this->parent_->set_encoder_position(this->last_position_);
  }
  if (this->last_position_ > this->max_value_) {
    this->last_position_ = this->max_value_;
    this->parent_->set_encoder_position(this->last_position_);
  }
  
  this->publish_state(this->last_position_);
}

void SeesawRotaryEncoderSensor::loop() {
  int32_t new_position = this->parent_->get_encoder_position();
  
  // Clamp to min/max if set
  if (new_position < this->min_value_) {
    new_position = this->min_value_;
    this->parent_->set_encoder_position(new_position);
  }
  if (new_position > this->max_value_) {
    new_position = this->max_value_;
    this->parent_->set_encoder_position(new_position);
  }
  
  // Only publish if changed
  if (new_position != this->last_position_) {
    this->last_position_ = new_position;
    this->publish_state(new_position);
  }
}

void SeesawRotaryEncoderSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Seesaw Rotary Encoder Sensor:");
  LOG_SENSOR("  ", "Encoder", this);
  if (this->min_value_ != INT32_MIN) {
    ESP_LOGCONFIG(TAG, "  Min Value: %d", this->min_value_);
  }
  if (this->max_value_ != INT32_MAX) {
    ESP_LOGCONFIG(TAG, "  Max Value: %d", this->max_value_);
  }
}

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
