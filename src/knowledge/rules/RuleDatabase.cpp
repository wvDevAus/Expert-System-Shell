#include "RuleDatabase.hpp"

namespace expert_system::knowledge::rules {

    void RuleDatabase::Run(facts::FactDatabase& database) {
        // Iterate through the map
        for (auto current_rule: managed_rules_) {
            // Call the Run operation, passing in the Rule's name
            current_rule.second.Run(database, current_rule.first);
        }
    }

} // namespace expert_system::knowledge::rules
