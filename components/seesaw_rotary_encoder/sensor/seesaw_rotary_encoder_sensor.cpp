#include "seesaw_rotary_encoder_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw_rotary_encoder {

static const char *const TAG = "seesaw_rotary_encoder.sensor";

void SeesawRotaryEncoderSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw Rotary Encoder Sensor...");
  
  // Enable encoder interrupt
  this->parent_->enable_encoder_interrupt();
  
  // Clamp initial position to valid range
  this->last_position_ = std::max(std::min(this->last_position_, this->max_value_), this->min_value_);
  
  // Reset hardware encoder position to match our software position
  // This ensures delta tracking starts from the correct baseline
  this->parent_->set_encoder_position(this->last_position_);
  
  this->publish_state(this->last_position_);
  this->last_update_ms_ = millis();
}

void SeesawRotaryEncoderSensor::loop() {
  // Throttle polling to reduce I2C bus traffic and improve stability
  // Poll every 1ms (1000Hz) which is responsive enough for encoder input
  uint32_t now = millis();
  // Use signed arithmetic to handle millis() rollover correctly
  if ((int32_t)(now - (int32_t)this->last_update_ms_) < 1) {
    return;
  }
  this->last_update_ms_ = now;
  
  // Read delta since last read (this is the proper way per Adafruit documentation)
  // Reading delta does NOT interfere with the hardware tracking like reading position does
  int32_t delta = this->parent_->get_encoder_delta();
  
  // Only process if there was actual movement
  if (delta != 0) {

    // Apply step scaling to the delta
    int32_t scaled_delta = static_cast<int32_t>(delta * this->step_);
    int32_t new_position = this->last_position_ + scaled_delta;

    ESP_LOGD(TAG, "Delta: %d, scaled: %d, old pos: %d, new pos (unclamped): %d", 
             delta, scaled_delta, this->last_position_, new_position);

    new_position = std::max(std::min(new_position, this->max_value_), this->min_value_);

    // Only publish if position actually changed after clamping
    if (new_position != this->last_position_) {
      this->last_position_ = new_position;
      this->publish_state(new_position);
      ESP_LOGI(TAG, "Encoder position: %d", new_position);
    }
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
  ESP_LOGCONFIG(TAG, "  Step: %.2f", this->step_);
}

}  // namespace seesaw_rotary_encoder
}  // namespace esphome
