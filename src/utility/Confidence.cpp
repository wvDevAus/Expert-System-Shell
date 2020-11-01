#pragma once

#include "Confidence.hpp"

namespace expert_system::utility {

    Confidence::Confidence() : confidence_factor_(0.0f) {}

    Confidence::Confidence(float value) {
        // Assign the value within bounds
        confidence_factor_ = 0.0f;
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

    Confidence Confidence::Combine(const Confidence& value) const {
        // Multiply the two values together
        return Confidence(confidence_factor_ * value.confidence_factor_);
    }

    void to_json(nlohmann::json& json_sys, const Confidence& target) {
        // Just save the current confidence factor to file
        json_sys = target.confidence_factor_;
    }

    void from_json(const nlohmann::json& json_sys, Confidence& target) {
        // Assign the default value to catch invalid data being gathered
        target.confidence_factor_ = 0.0f;

        // Check the stored data isn't empty
        if (!json_sys.empty()) {
            // Check the stored data is a float
            if (json_sys.is_number_float()) {
                // Store the data
                target.Set(json_sys.get<float>());
            }
        }
    }

} // namespace expert_system::utility
