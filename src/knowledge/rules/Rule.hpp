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
        Rule() = default;

            /**
             * @brief Gathers the identifiers of the Facts required to trigger this Rule.
             * @return A set of the Antecedent's target Fact names, or std::nullopt if the Rule's Antecedent is invalid.
             * @note All of these Facts must have a known session value to trigger the Rule.
             * @warning No tests will be performed to check that these Fact names are valid!
             */
        std::optional<std::set<std::string>> TriggerFacts();

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
             * @param [in] source The name of this Rule (stored externally).
             * @return A TestOutcome result paired with a confidence factor.
             * @note This will return an error symbol if a Condition could not be operated.
             * @warning A TestOutcome of KComparisonSuccess may hide Consequent operation failure!
             */
        std::pair<TestOutcome, utility::Confidence> Run(facts::FactDatabase& database,
                                                        std::string source);

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

    private:
            /**
             * @brief A filtered string containing a description of the Fact.
             * May be completely empty.
             */
        std::string description_;
    };

}
