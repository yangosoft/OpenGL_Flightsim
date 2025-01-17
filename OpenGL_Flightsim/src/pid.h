#pragma once

#include <glm/vec2.hpp>

class PID {
  float integral = 0.0f;
  bool initialized = false;
  float previous_value = 0.0f, previous_error = 0.0f;

  const bool use_value;
  const glm::vec2 output_range{-1.0f, 1.0f};
  const float proportional_gain, integral_gain, derivative_gain;

  void reset() { initialized = false; }

 public:
  PID(float kp, float ki, float kd, bool use_value = true)
      : proportional_gain(kp), integral_gain(ki), derivative_gain(kd), use_value(use_value) {}

  float calculate(float current_value, float target_value, float dt) {
    float error = target_value - current_value;
    float P = error * proportional_gain;

    if (!initialized) {
      previous_error = error;
      previous_value = current_value;
    }

    integral += error * dt;
    float I = integral * integral_gain;

    float error_rate_of_change = (error - previous_error) / dt;
    previous_error = error;

    float value_rate_of_change = (current_value - previous_value) / dt;
    previous_value = current_value;

    float D = (use_value ? value_rate_of_change : error_rate_of_change) * derivative_gain;

    return glm::clamp(P + I + D, output_range.x, output_range.y);
  }
};
