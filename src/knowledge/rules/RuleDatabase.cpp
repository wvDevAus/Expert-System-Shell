#include "RuleDatabase.hpp"

namespace expert_system::knowledge::rules {

    void RuleDatabase::Run(facts::FactDatabase& database) {
        // Iterate through the map
        for (auto current_rule: managed_rules_) {
            // Call the Run operation, passing in the Rule's name
            current_rule.second.Run(database, current_rule.first);
        }
    }

    void to_json(nlohmann::json& json_sys, const RuleDatabase& target) {
        // Export the list of Rules
        json_sys = target.managed_rules_;
    }

    void from_json(const nlohmann::json& json_sys, RuleDatabase& target) {
        // Attempt to gather the Rules
        if (!json_sys.empty()) {
            // Store the mapped Rules
            target.managed_rules_ = json_sys.get<std::map<std::string, Rule>>();
        }
    }

} // namespace expert_system::knowledge::rules
