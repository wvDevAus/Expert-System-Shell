#pragma once

#include <optional>
#include <chrono>

namespace expert_system {

        /**
         * @brief A session value for a Fact, with additional metadata.
         * @tparam T The raw type of the Fact.
         */
    template <class T>
    struct Value {
            /**
             * @brief Parameterized constructor, automatically assigns metadata.
             * @param [in] value The session value for the Fact.
             * @param [in] confidence_factor A specifier of the session value's confidence factor.
             * @param [in] source_event The identifier of the event that generated the session value.
             */
        Value(T value, float confidence_factor,
                  std::optional<int> source_event = std::nullopt);

            /// The session value of a Fact.
        T value_;

            /**
             * @brief A specifier of the session value's confidence.
             * @note Only valid if between 0.0f (no confidence) and 1.0f (full confidence).
             */
        float confidence_factor_;

            /// The time that a Fact's session value was constructed.
        std::chrono::time_point<std::chrono::system_clock> timestamp_;

            ///The identifier of the event that generated the session value.
        int source_event_;
    };

    Value(T value, float confidence_factor, std::optional<int> source_event)
            : value_(value), confidence_factor_(confidence_factor),
                source_event_(source_event) {
        // Generate and store a timestamp
        timestamp_ = std::chrono::system_clock::now();
    }

} // expert_system
