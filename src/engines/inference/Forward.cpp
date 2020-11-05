#include "Forward.hpp"

#include <algorithm>
#include <set>

#include "knowledge/facts/Values.hpp"

namespace expert_system::engines::inference::forward {

    explanation::Log LogExisting(knowledge::facts::FactDatabase& fact_database) {
        // Create a log to store all of the existing session values
        explanation::Log data_log;

        // Get the identifiers of all of the Facts that have a session value
        auto facts_with_value = fact_database.List(knowledge::facts::FactFilter::kHasValue);

        // For each of the gathered identifiers
        for (auto& current_fact_name: facts_with_value) {
            // Get the actual Fact
            auto search_result = fact_database.Find(current_fact_name);

            // Catch if the returned Fact is actually valid
            if (search_result.has_value()) {
                // Store the Fact name and Value in the log
                data_log.assignments_.emplace(
                        current_fact_name,
                        knowledge::facts::VariantValue(search_result.value().get()));
            }
        }

        // Return the log
        return data_log;
    }

    std::optional<explanation::Log> LogSpecific(
            std::string fact_name,
            knowledge::facts::FactDatabase& fact_database) {
        // Check if the target Fact actually has a known session value
        if (!fact_database.Known(fact_name)) {
            // Catch the error and indicate failure
            return std::nullopt;
        }

        // Gather the Fact and generate a log
        explanation::Log specific_log;
        knowledge::facts::VariantValue specific_value(fact_database.Find(fact_name).value().get());
        specific_log.assignments_.emplace(fact_name, specific_value);
        return specific_log;
    }

    std::set<std::string> IdentifyFactRequests(
            knowledge::facts::FactDatabase& fact_database,
            knowledge::rules::RuleDatabase& rule_database) {
        // Keep track of the Facts for values that could be requested from the user
        std::set<std::string> total_requests;

        // Get the identifiers of all of the Facts that lack a session value
        auto facts_without_value = fact_database.List(knowledge::facts::FactFilter::kHasValue);

        // Iterate through the rules that have not yet been run
        auto unused_rules = rule_database.ListRules(
                knowledge::rules::RuleFilter::kHasNotRunConsequent);
        for (const auto& current_rule: unused_rules) {
            // Get the Facts that would be required to trigger this Rule
            auto rule_requirements = current_rule->second.TriggerFacts();

            // Identify which Facts could be requested from the user
            std::set<std::string> rule_requests;
            std::set_union(facts_without_value.begin(), facts_without_value.end(),
                           rule_requirements.begin(), rule_requirements.end(),
                           std::inserter(rule_requests, rule_requests.begin()));

            // Add these Facts to the combined target requests
            std::set<std::string> temp_total_requests;
            std::merge(total_requests.begin(), total_requests.end(),
                       rule_requests.begin(), rule_requests.end(),
                       std::inserter(temp_total_requests, temp_total_requests.begin()));
            total_requests = temp_total_requests;
        }

        // Return the set of requests
        return total_requests;
    }

    std::list<std::map<std::string, knowledge::rules::Rule>::iterator> FindTriggered(
            knowledge::facts::FactDatabase& fact_database,
            knowledge::rules::RuleDatabase& rule_database) {
        // Keep track of the triggered Rules
        std::list<std::map<std::string, knowledge::rules::Rule>::iterator> triggered_rules;

        // Iterate through the Rules that have not yet been run
        auto unused_rules = rule_database.ListRules(
                knowledge::rules::RuleFilter::kHasNotRunConsequent);
        for (const auto& current_rule: unused_rules) {
            // Check if the current Rule has been triggered
            if (current_rule->second.trigger_.Test(fact_database)
                == knowledge::rules::TestOutcome::kComparisonSuccess) {
                // Store the current Rule for future reference
                triggered_rules.emplace_back(current_rule);
            }
        }

        // Return the triggered Rules
        return triggered_rules;
    }

    std::vector<explanation::Log> RunTriggered(
            std::list<std::map<std::string, knowledge::rules::Rule>::iterator> triggered_rules,
            knowledge::facts::FactDatabase& fact_database) {
        // Create a log for each Consequent run
        std::vector<explanation::Log> trigger_log;

        // Iterate through the triggered rules
        for (const auto& current_fact: triggered_rules) {
            // Attempt the Rule's assignment and track the results
            auto assignment_outcomes = current_fact->second.response_.Assign(fact_database);

            // Create a Log for this Rule's attempted assignment
            explanation::Log current_fact_log;
            current_fact_log.rule_ = current_fact;

            // Iterate through the outcomes of the assignments
            for (auto& current_outcome: assignment_outcomes) {
                // Gather the assignment's Fact
                auto assignment_fact = fact_database.Find(current_outcome.first).value();

                // Add the assigned data to the Log
                current_fact_log.assignments_.emplace(current_outcome.first, assignment_fact);
            }

            // Add this log to the collection
            trigger_log.push_back(current_fact_log);
        }

        // Return the logs from the triggered outcomes
        return trigger_log;
    }

} // namespace expert_system::engines::inference::forward
