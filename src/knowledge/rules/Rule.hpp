#pragma once

#include <optional>
#include <set>
#include <string>

#include "knowledge/rules/Antecedent.hpp"
#include "knowledge/rules/Consequent.hpp"

namespace expert_system::knowledge::rules {

        /**
         * @brief An expert system Rule, composed of a Antecedent and Consequent.
         * Manages the process of running individual expert system Rules.
         */
    class Rule {
    public:
            /// Default constructor
        Rule();

            /**
             * @brief Gathers the identifiers of the Facts required to trigger this Rule.
             * @return A set of the Antecedent's target Fact names, or an empty set if the Rule's Antecedent is invalid.
             * @note All of these Facts must have a known session value to trigger the Rule.
             * @warning No tests will be performed to check that these Fact names are valid!
             */
        std::set<std::string> TriggerFacts();

            /**
             * @brief Gathers the identifiers of the Facts that would be assigned a session value by this Rule.
             * @return A set of the Consequent's target Fact names.
             * @note This may list Facts that already have a session value.
             * @warning No tests will be performed to check that these Fact names are valid!
             */
        std::set<std::string> ResponseFacts();

            /**
             * @brief Test's the Rule's Antecedent and runs its Consequent on a successful trigger.
             * @param [in] database The Fact Database to operate on.
             * @return A TestOutcome enum symbol indicating the test's result.
             * @note This will return an error symbol if a Condition could not be operated.
             * @warning A TestOutcome of KComparisonSuccess may hide Consequent operation failure!
             */
        TestOutcome Run(facts::FactDatabase& database);

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

            /// The trigger component of the expert system Rule.
        Antecedent trigger_;

            /// The response component of the expert system Rule.
        Consequent response_;

            /// Tracks if the Rule has been used in inference yet.
        bool already_applied_;

    private:
            /**
             * @brief A filtered string containing a description of the Fact.
             * May be completely empty.
             */
        std::string description_;

            /// Enables JSON serializer access to private contents
        friend void to_json(nlohmann::json& json_sys, const Rule& target);

            /// Enables JSON serializer access to private contents
        friend void from_json(const nlohmann::json& json_sys, Rule& target);
    };

        /**
         * @brief Rule serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the Rule to export.
         */
    void to_json(nlohmann::json& json_sys, const Rule& target);

        /**
         * @brief Rule serialization from JSON format.
         * @param [in] json_sys A reference to a JSON object.
         * @param [in,out] target A reference to the Rule to import.
         */
    void from_json(const nlohmann::json& json_sys, Rule& target);

}
