#include "Antecedent.hpp"

namespace expert_system::knowledge::rules {

    std::pair<TestOutcome, utility::Confidence> Antecedent::Test(facts::FactDatabase& source) {
        // Attempt to Test the root Condition and keep track of the outcomes
        auto combined_outcomes = root_condition_.Test(source);

        // Catch an error output
        if ((combined_outcomes.first != TestOutcome::kComparisonSuccess)
            && (combined_outcomes.first != TestOutcome::kComparisonFailure)) {
            // Stop and return the error output
            return combined_outcomes;
        }

        // Iterate through the list in order and perform each Test
        for (auto current_pair: condition_chain_) {
            // Keep track of the current Condition's outcome
            auto local_outcome = current_pair.second.Test(source);

            // Catch an error output
            if ((local_outcome.first != TestOutcome::kComparisonSuccess)
                    && (local_outcome.first != TestOutcome::kComparisonFailure)) {
                // Stop and return the error output
                return local_outcome;
            }

            // Chain the outcome with the specified ConnectorType
            switch (current_pair.first) {
                case ConnectorType::kAnd: {
                    // Both outcomes must be successful
                    if ((local_outcome.first != TestOutcome::kComparisonSuccess)
                        || (combined_outcomes.first != TestOutcome::kComparisonSuccess)) {
                        // Store failure in the chained result
                        combined_outcomes.first = TestOutcome::kComparisonFailure;
                        combined_outcomes.second = combined_outcomes.second.Combine(local_outcome.second);
                    }
                }
                case ConnectorType::kOr: {
                    // Either outcome must be successful
                    if ((local_outcome.first == TestOutcome::kComparisonSuccess)
                            || (combined_outcomes.first == TestOutcome::kComparisonSuccess)) {
                        // Store success in the chained result
                        combined_outcomes.first = TestOutcome::kComparisonFailure;
                        combined_outcomes.second = combined_outcomes.second.Combine(local_outcome.second);
                    }
                }
                case ConnectorType::kXor: {
                    // The outcomes cannot be identical
                    if (local_outcome.first == combined_outcomes.first) {
                        // Store failure in the chained result
                        combined_outcomes.first = TestOutcome::kComparisonFailure;
                        combined_outcomes.second = combined_outcomes.second.Combine(local_outcome.second);
                    } else {
                        // Store success in the chained result
                        combined_outcomes.first = TestOutcome::kComparisonSuccess;
                        combined_outcomes.second = combined_outcomes.second.Combine(local_outcome.second);
                    }
                }
            }
        }

        // Return the final chained outcome
        return combined_outcomes;
    }

} // namespace expert_system::knowledge::rules
