#pragma once

#include <list>
#include <optional>
#include <string>

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/Assignments.hpp"
#include "utility/Confidence.hpp"

namespace expert_system::knowledge::rules {

        /**
         * @brief The response component of an expert system Rule.
         * Composed of one-to-many Assignments.
         */
    struct Consequent {
            /// Default constructor.
        Consequent() = default;

            /**
             * @brief Attempts to perform the stored Assignments.
             * @param [in] database The Fact Database to operate on.
             * @param [in] confidence_factor The confidence factor from the triggered Condition.
             * @warning Assignments may fail, and will do so silently!
             */
        void Assign(facts::FactDatabase& database,
                    utility::Confidence& confidence_factor);

            /**
             * @brief The list of Assignments to activate.
             * The Assignments will be assigned in the order of their storage in the list.
             */
        std::list<VariantAssignment> assignments_;
    };

} // namespace expert_system::knowledge::rules
