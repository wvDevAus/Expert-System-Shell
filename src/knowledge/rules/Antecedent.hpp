#pragma once

#include <tuple>
#include <list>

#include "nlohmann/json.hpp"

#include "knowledge/rules/Condition.hpp"
#include "knowledge/rules/Conditions.hpp"

namespace expert_system::knowledge::rules {

        /// The symbolic representations for logical connections between Conditions.
    enum class ConnectorType {
        kAnd,
        kOr,
        kXor
    };

        /// JSON serialization for the ConnectorType enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(ConnectorType,
                                {{ConnectorType::kAnd, "AND"},
                                 {ConnectorType::kOr, "OR"},
                                 {ConnectorType::kXor, "XOR"}})

        /**
         * @brief The trigger component of an expert system Rule.
         * Composed of one-to-many Conditions linked with logical connections.
         */
    struct Antecedent {
            /// Default constructor
        Antecedent() = default;

            /**
             * @brief Test the logically linked Conditions.
             * @param [in] source The Fact Database to operate on.
             * @return A TestOutcome result paired with a confidence factor.
             * @note This will return an error symbol if a Condition could not be operated.
             * @warning No indication will be given for the source of the error!
             */
        TestOutcome Test(facts::FactDatabase& source);

            /**
             * @brief The root Condition to begin the test upon.
             * All Condition chaining will occur after this.
             */
        VariantCondition root_condition_;

            /**
             * @brief The list of Conditions to chain Tests across.
             * The current result will be chained to next Condition's result.
             */
        std::list<std::pair<ConnectorType, VariantCondition>> condition_chain_;
    };

        /**
         * @brief Antecedent serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the Antecedent to export.
         */
    void to_json(nlohmann::json& json_sys, const Antecedent& target);

        /**
         * @brief Antecedent serialization from JSON format.
         * @param [in] json_sys A reference to a JSON object.
         * @param [in,out] target A reference to the Antecedent to import.
         */
    void from_json(const nlohmann::json& json_sys, Antecedent& target);

} // namespace expert_system::knowledge::rules
