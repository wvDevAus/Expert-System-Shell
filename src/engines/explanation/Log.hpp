#pragma once

#include <optional>
#include <map>
#include <string>

#include "knowledge/facts/Facts.hpp"
#include "knowledge/facts/Values.hpp"
#include "knowledge/rules/Rule.hpp"

namespace expert_system::engines::explanation {

        /**
         * @brief A data structure used to keep track of the Fact session value assignment.
         * Stores a copy of the raw data at the time of its assignment.
         */
    struct Log {
            /**
             * @brief Default constructor.
             * Sets rule_ to std::nullopt.
             */
        Log();

            /**
             * @brief Stores a copy of the assigned data.
             * Maps the Fact's copied session Value to its name.
             */
        std::map<std::string, knowledge::facts::VariantValue> assignments_;

            /**
             * @brief Stores the iterator to the Rule that caused the Event.
             * @note Contains std::nullopt if the event was caused by a user.
             */
        std::optional<std::map<std::string, knowledge::rules::Rule>::iterator> rule_;
    };

} // namespace expert_system::engines::explanation
