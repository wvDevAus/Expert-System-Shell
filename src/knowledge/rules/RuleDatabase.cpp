#include "RuleDatabase.hpp"

namespace expert_system::knowledge::rules {

    void RuleDatabase::Run(facts::FactDatabase& database) {
        // Iterate through the map
        for (auto current_rule: managed_rules_) {
            // Call the Run operation, passing in the Rule's name
            current_rule.second.Run(database);
        }
    }

    std::list<std::map<std::string, Rule>::iterator> RuleDatabase::ListRules(RuleFilter filter) {
        // Keep track of the iterators
        std::list<std::map<std::string, Rule>::iterator> filtered_rules;

        // Iterate through the stored rules
        for (auto current_rule = managed_rules_.begin(); current_rule != managed_rules_.end(); ++current_rule) {
            // Split logic based on the requested filter
            switch (filter) {
                case RuleFilter::kAll: {
                    // Just add the iterator to the list
                    filtered_rules.emplace_back(current_rule);
                    break;
                }
                case RuleFilter::kHasNotRunConsequent: {
                    // Check the Rule's flag member
                    if (!current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        filtered_rules.emplace_back(current_rule);
                    }
                    break;
                }
                case RuleFilter::kHasRunConsequent: {
                    // Check the Rule's flag member
                    if (current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        filtered_rules.emplace_back(current_rule);
                    }
                    break;
                }
                default: {
                    // Do nothing
                    break;
                }
            }
        }

        // Return the list of Rule iterators
        return filtered_rules;
    }

    std::set<std::string> RuleDatabase::ListRuleAntedecentFacts(
            RuleFilter filter) {
        // Keep track of the Fact names
        std::set<std::string> fact_names;

        // Iterate through the stored rules
        for (auto current_rule = managed_rules_.begin(); current_rule != managed_rules_.end(); ++current_rule) {
            // Split logic based on the requested filter
            switch (filter) {
                case RuleFilter::kAll: {
                    // Just add the iterator to the list
                    fact_names.merge(current_rule->second.TriggerFacts());
                    break;
                }
                case RuleFilter::kHasNotRunConsequent: {
                    // Check the Rule's flag member
                    if (!current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        fact_names.merge(current_rule->second.TriggerFacts());
                    }
                    break;
                }
                case RuleFilter::kHasRunConsequent: {
                    // Check the Rule's flag member
                    if (current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        fact_names.merge(current_rule->second.TriggerFacts());
                    }
                    break;
                }
            }
        }

        // Return the list of Fact names
        return fact_names;
    }

    std::set<std::string> RuleDatabase::ListRuleConsequentFacts(
            RuleFilter filter) {
        // Keep track of the Fact names
        std::set<std::string> fact_names;

        // Iterate through the stored rules
        for (auto current_rule = managed_rules_.begin(); current_rule != managed_rules_.end(); ++current_rule) {
            // Split logic based on the requested filter
            switch (filter) {
                case RuleFilter::kAll: {
                    // Just add the iterator to the list
                    fact_names.merge(current_rule->second.ResponseFacts());
                }
                case RuleFilter::kHasNotRunConsequent: {
                    // Check the Rule's flag member
                    if (!current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        fact_names.merge(current_rule->second.ResponseFacts());
                    }
                }
                case RuleFilter::kHasRunConsequent: {
                    // Check the Rule's flag member
                    if (current_rule->second.successful_response_) {
                        // Add the iterator to the list
                        fact_names.merge(current_rule->second.ResponseFacts());
                    }
                }
            }
        }

        // Return the list of Fact names
        return fact_names;
    }

    void RuleDatabase::ResetRules() {
        // Iterate through all of the Rules
        for (auto current_rule: managed_rules_) {
            // Just set the flag to false
            current_rule.second.successful_response_ = false;
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
