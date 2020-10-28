#pragma once

#include <optional>
#include <string>

#include "databases/facts/Range.hpp"
#include "databases/facts/Value.hpp"

namespace expert_system {

        /**
         * @brief Abstract Fact parent, used as the interface for all Facts.
         * @tparam T The raw type of the Fact.
         */
    template <class T>
    class Fact {
    public:
            /**
             * Default constructor.
             * Assigns std::nullopt to all optional members.
             * Assigns an empty string to the Fact's description.
             */
        Fact();

            /**
             * @brief Assigns a Range to the Fact.
             * This will be used to check if a provided session value is valid.
             * @param [in] min The Range's minimum value.
             * @param [in] max The Range's maximum value.
             * @param [in] bounds_inclusive Flag for including the bounds of the Range.
             * @return True if the Range is valid and no session value is assigned, False otherwise.
             * @note The Fact's Range will not be modified if this returns False.
             */
        bool SetRange(T min, T max, bool bounds_inclusive);

            /**
             * @brief Gather a copy of the Fact's Range.
             * @return A copy of range_.
             * @note If the Fact has no Range, this will return std::nullopt.
             */
        std::optional<Range<T>> GetRange();

            /**
             * @brief Removes the Fact's Range.
             * @note This will have no effect if the fact does not have a Range assigned.
             */
        void ClearRange();

            /**
             * @brief Updates the Fact's description.
             * @param [in] description A string containing a description of the Fact.
             * @note Guarantees the description will be filtered.
             */
        void SetDescription(std::string description);

            /**
             * @brief Gathers the Fact's description.
             * @return A copy of description_.
             */
        std::string GetDescription();

            /**
             * @brief Assigns a session value to the Fact.
             * @param [in] value The session value for the Fact.
             * @param [in] confidence_factor A specifier of the session value's confidence factor.
             * @param [in] source_event The identifier of the event that generated the session value.
             * @return True if the provided value is within the Fact's Range, False otherwise.
             * @note If the Fact has no Range, this is guaranteed to always return True.
             */
        bool SetValue(T value, float confidence_factor,
                      std::optional<int> source_event = std::nullopt);

            /**
             * @brief Gather a copy of the Fact's session value.
             * @return A copy of value_.
             * @note If the Fact has no session value, this will return std::nullopt.
             */
        std::optional<Value<T>> GetValue();

            /**
             * @brief Removes the Fact's session value.
             * @note This will have no effect if the fact does not have a session value assigned.
             */
        void ClearValue();

    private:
            /**
             * @brief The Fact's Range.
             * Used to limit the possible set of session values.
             */
        std::optional<Range<T>> range_;

            /**
             * @brief A filtered string containing a description of the Fact.
             * May be completely empty.
             */
        std::string description_;

            /**
             * @brief The Fact's session value.
             * Contains the Fact's raw value and metadata.
             */
        std::optional<Value<T>> value_;
    };

} // expert_system
