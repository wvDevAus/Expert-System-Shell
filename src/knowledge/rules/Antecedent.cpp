#include "Antecedent.hpp"

namespace expert_system::knowledge::rules {

    TestOutcome Antecedent::Test(facts::FactDatabase& source) {
        // Attempt to Test the root Condition and keep track of the outcomes
        TestOutcome combined_outcomes = root_condition_.Test(source);

        // Catch an error output
        if ((combined_outcomes != TestOutcome::kComparisonSuccess)
            && (combined_outcomes != TestOutcome::kComparisonFailure)) {
            // Stop and return the error output
            return combined_outcomes;
        }

        // Iterate through the list in order and perform each Test
        for (auto current_pair: condition_chain_) {
            // Keep track of the current Condition's outcome
            TestOutcome local_outcome = current_pair.second.Test(source);

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
                    if ((local_outcome != TestOutcome::kComparisonSuccess)
                        || (combined_outcomes != TestOutcome::kComparisonSuccess)) {
                        // Store failure in the chained result
                        combined_outcomes = TestOutcome::kComparisonFailure;
                    }
                }
                case ConnectorType::kOr: {
                    // Either outcome must be successful
                    if ((local_outcome == TestOutcome::kComparisonSuccess)
                            || (combined_outcomes == TestOutcome::kComparisonSuccess)) {
                        // Store success in the chained result
                        combined_outcomes = TestOutcome::kComparisonFailure;
                    }
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
                }
            }
        }

        // Return the final chained outcome
        return combined_outcomes;
    }

} // namespace expert_system::knowledge::rules
