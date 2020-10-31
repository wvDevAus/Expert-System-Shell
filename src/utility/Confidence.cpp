#pragma once

#include "Confidence.hpp"

namespace expert_system::utility {

    Confidence::Confidence() : confidence_factor_(0.0f) {}

    Confidence::Confidence(float value) {
        // Assign the value within bounds
        Set(value);
    }

    void Confidence::Set(float value) {
        // Catch if the provided value is above the maximum
        if (1.0f < value) {
            // Store the maximum value
            confidence_factor_ = 1.0f;
            return;
        }

        // Catch if the provided value is below the minimum
        if (value < 0.0f) {
            // Store the maximum value
            confidence_factor_ = 0.0f;
            return;
        }

        // Store the provided value
        confidence_factor_ = value;
    }

    float Confidence::Get() {
        // Return a copy of the stored value
        return confidence_factor_;
    }

    Confidence Confidence::Combine(Confidence& value) {
        // Multiply the two values together
        return Confidence(confidence_factor_ * value.confidence_factor_);
    }

} // namespace expert_system::utility
