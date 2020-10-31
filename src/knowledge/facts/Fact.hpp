#pragma once

#include <optional>
#include <string>

#include "knowledge/facts/Value.hpp"
#include "utility/Range.hpp"

namespace expert_system::knowledge::facts {

        /**
         * @brief Template Fact.
         * @tparam T The raw type of the Fact.
         */
    template<class T>
    class Fact {
    public:
            /**
             * Default constructor.
             * Assigns std::nullopt to all optional members.
             * Assigns an empty string to the Fact's description.
             */
        Fact() : range_(std::nullopt), value_(std::nullopt) {};

            /**
             * @brief Assigns a Range to the Fact.
             * This will be used to check if a provided session value is valid.
             * @param [in] min The Range's minimum value.
             * @param [in] max The Range's maximum value.
             * @param [in] bounds_inclusive Flag for including the bounds of the Range.
             * @return True if the Range is valid and no session value is assigned, False otherwise.
             * @note The Fact's Range will not be modified if this returns False.
             */
        bool SetRange(T min, T max, bool bounds_inclusive) {
            // Catch a session value existing
            if (value_.has_value()) {
                // Catch possible invalidation of session value and stop
                return false;
            }

            // Check if the proposed range minimum is larger than the maximum
            if (max < min) {
                // Catch invalid range and stop
                return false;
            }

            // Generate and store a new Range
            range_.emplace(min, max, bounds_inclusive);
            return true;
        };

            /**
             * @brief Gather a copy of the Fact's Range.
             * @return A copy of range_.
             * @note If the Fact has no Range, this will return std::nullopt.
             */
        std::optional<utility::Range<T>> GetRange() {
            // Just return a copy of range_, not the contents inside of it.
            return range_;
        };

            /**
             * @brief Removes the Fact's Range.
             * @note This will have no effect if the fact does not have a Range assigned.
             */
        void ClearRange() {
            // Destroy the contents of the range_ member.
            range_.reset();
        };

            /**
             * @brief Updates the Fact's description.
             * @param [in] description A string containing a description of the Fact.
             * @note Guarantees the description will be filtered.
             */
        void SetDescription(std::string description) {
            // Trim the whitespace from the front of the description
            description.erase(description.begin(),
                              std::find_if(description.begin(), description.end(),
                                           [](unsigned char ch) { return !std::isspace(ch); }));

            // Trim the whitespace from the back of the description
            description.erase(
                    std::find_if(description.rbegin(), description.rend(),
                                 [](unsigned char ch) { return !std::isspace(ch); })
                            .base(),
                    description.end());

            // Store the trimmed description string
            description_ = description;
        };

            /**
             * @brief Gathers the Fact's description.
             * @return A copy of description_.
             */
        std::string GetDescription() {
            // Simply provide a copy of the stored string
            return description_;
        };

            /**
             * @brief Assigns a session value to the Fact.
             * @param [in] value The session value for the Fact.
             * @param [in] confidence_factor A specifier of the session value's confidence factor.
             * @param [in] source_event The identifier of the event that generated the session value.
             * @return True if the provided value is within the Fact's Range, False otherwise.
             * @note If the Fact has no Range, this is guaranteed to always return True.
             */
        bool SetValue(T value, float confidence_factor,
                      std::optional<int> source_event = std::nullopt) {
            // Check if the Fact has an assigned range
            if (range_.has_value()) {
                // Check if the provided value is not valid
                if (!Range<T>::InRange(range_.value(), value)) {
                    // Catch an invalid value and stop
                    return false;
                }
            }

            // Create the new session value
            value_.emplace(value, confidence_factor, source_event);
        };

            /**
             * @brief Gather a copy of the Fact's session value.
             * @return A copy of value_.
             * @note If the Fact has no session value, this will return std::nullopt.
             */
        std::optional<utility::Value<T>> GetValue() {
            // Just return a copy of value_.
            return value_;
        };

            /**
             * @brief Removes the Fact's session value.
             * @note This will have no effect if the fact does not have a session value assigned.
             */
        void ClearValue() {
            // Destroy the contents of the value_ member.
            value_.reset();
        };

    private:
            /**
             * @brief The Fact's Range.
             * Used to limit the possible set of session values.
             */
        std::optional<utility::Range<T>> range_;

            /**
             * @brief A filtered string containing a description of the Fact.
             * May be completely empty.
             */
        std::string description_;

            /**
             * @brief The Fact's session value.
             * Contains the Fact's raw value and metadata.
             */
        std::optional<utility::Value<T>> value_;
    };

}// namespace expert_system::knowledge::facts
