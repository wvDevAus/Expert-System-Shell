#include "Antecedent.hpp"

#include "utility/SymbolsJSON.hpp"

namespace expert_system::knowledge::rules {

    TestOutcome Antecedent::Test(facts::FactDatabase& source) {
        // Attempt to Test the root Condition and keep track of the outcomes
        auto combined_outcomes = root_condition_.Test(source);

        // Catch an error output
        if ((combined_outcomes != TestOutcome::kComparisonSuccess)
            && (combined_outcomes != TestOutcome::kComparisonFailure)) {
            // Stop and return the error output
            return combined_outcomes;
        }

        // Iterate through the list in order and perform each Test
        for (auto current_pair: condition_chain_) {
            // Keep track of the current Condition's outcome
            auto local_outcome = current_pair.second.Test(source);

            // Catch an error output
            if ((local_outcome != TestOutcome::kComparisonSuccess)
                    && (local_outcome != TestOutcome::kComparisonFailure)) {
                // Stop and return the error output
                return local_outcome;
            }

            // Chain the outcome with the specified ConnectorType
            switch (current_pair.first) {
                case ConnectorType::kAnd: {
                    // Both outcomes must be successful
                    if ((local_outcome == TestOutcome::kComparisonSuccess)
                        && (combined_outcomes == TestOutcome::kComparisonSuccess)) {
                        // Store success in the chained result
                        combined_outcomes = TestOutcome::kComparisonSuccess;
                    }
                    else {
                        // Store failure in the chained result
                        combined_outcomes = TestOutcome::kComparisonFailure;
                    }
                    break;
                }
                case ConnectorType::kOr: {
                    // Either outcome must be successful
                    if ((local_outcome == TestOutcome::kComparisonSuccess)
                            || (combined_outcomes == TestOutcome::kComparisonSuccess)) {
                        // Store success in the chained result
                        combined_outcomes = TestOutcome::kComparisonSuccess;
                    }
                    else {
                        // Store failure in the chained result
                        combined_outcomes = TestOutcome::kComparisonFailure;
                    }
                    break;
                }
                case ConnectorType::kXor: {
                    // The outcomes cannot be identical
                    if (local_outcome == combined_outcomes) {
                        // Store failure in the chained result
                        combined_outcomes = TestOutcome::kComparisonFailure;
                    } else {
                        // Store success in the chained result
                        combined_outcomes = TestOutcome::kComparisonSuccess;
                    }
                    break;
                }
            }
        }

        // Return the final chained outcome
        return combined_outcomes;
    }

    void to_json(nlohmann::json& json_sys, const Antecedent& target) {
        // Export the root Condition
        json_sys[utility::JSON_ID_ROOT] = target.root_condition_;

        // Export the chained Conditions
        json_sys[utility::JSON_ID_CHAIN] = target.condition_chain_;
    }

    void from_json(const nlohmann::json& json_sys, Antecedent& target) {
        // Clear the existing data
        target.root_condition_ = VariantCondition();
        target.condition_chain_.clear();

        // Confirm the description is actually stored
        if (json_sys.find(utility::JSON_ID_DESCRIPTION) != json_sys.end()) {
            // Attempt to gather the description
            if (!json_sys.at(utility::JSON_ID_DESCRIPTION).empty()) {
                // Store the description
                target.root_condition_ = json_sys.at(utility::JSON_ID_DESCRIPTION).get<VariantCondition>();
            }
        }

        // Confirm the root condition is actually stored
        if (json_sys.find(utility::JSON_ID_ROOT) != json_sys.end()) {
            // Attempt to gather the root condition
            if (!json_sys.at(utility::JSON_ID_ROOT).empty()) {
                // Store the root condition
                target.root_condition_ = json_sys.at(utility::JSON_ID_ROOT).get<VariantCondition>();
            }
        }

        // Confirm the condition chain is actually stored
        if (json_sys.find(utility::JSON_ID_CHAIN) != json_sys.end()) {
            // Attempt to gather the condition chain
            if (!json_sys.at(utility::JSON_ID_CHAIN).empty()) {
                // Store the condition chain
                target.condition_chain_ = json_sys.at(utility::JSON_ID_CHAIN).get<std::list<std::pair<ConnectorType, VariantCondition>>>();
            }
        }
    }

} // namespace expert_system::knowledge::rules
