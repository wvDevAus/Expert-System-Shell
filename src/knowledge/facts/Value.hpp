#pragma once

#include <chrono>
#include <functional>
#include <optional>

#include "utility/Confidence.hpp"

namespace expert_system::knowledge::facts {

        /**
         * @brief A session value for a Fact, with additional metadata.
         * @tparam T The raw type of the Fact.
         */
    template<class T>
    struct Value {
            /// Use of the default constructor is not allowed!
        Value() = delete;

            /**
             * @brief Parameterized constructor, automatically assigns metadata.
             * @param [in] value The session value for the Fact.
             * @param [in] confidence_factor A specifier of the session value's confidence factor.
             */
        Value(T value, utility::Confidence& confidence_factor)
            : value_(value), confidence_factor_(confidence_factor) {
            // Generate and store a timestamp
            timestamp_ = std::chrono::system_clock::now();
        };

            /// The session value of a Fact.
        T value_;

            /// A specifier of the session value's confidence.
        utility::Confidence confidence_factor_;

            /// The time that a Fact's session value was constructed.
        std::chrono::time_point<std::chrono::system_clock> timestamp_;
    };

} // namespace expert_system::knowledge::facts
