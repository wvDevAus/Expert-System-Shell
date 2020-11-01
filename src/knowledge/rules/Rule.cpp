#include "Rule.hpp"

#include <variant>

namespace expert_system::knowledge::rules {

    std::optional<std::set<std::string>> Rule::TriggerFacts() {
        // Keep track of the Fact names
        std::set<std::string> fact_names;

        // Check if the Rule's root Condition actually has an assigned target Fact
        auto root_condition_fact = trigger_.root_condition_.Fact();
        if (!root_condition_fact.has_value()) {
            // Catch the invalid Condition and indicate the Antecedent is invalid
            return std::nullopt;
        }

        // Store the root Condition's target Fact name
        fact_names.insert(trigger_.root_condition_.Fact().value());

        // Iterate through the Antecedent's chain of Conditions
        for (auto current_condition: trigger_.condition_chain_) {
            // Attempt to gather the current Condition's
            auto condition_fact_name = current_condition.second.Fact();

            // Check that the current Condition actually has an assigned target Fact
            if (!condition_fact_name.has_value()) {
                // Catch the invalid Condition and indicate the Antecedent is invalid
                return std::nullopt;
            }

            // Store the current Condition's target Fact name
            fact_names.insert(condition_fact_name.value());
        }

        // Return the set of Fact names
        return fact_names;
    }

    std::set<std::string> Rule::ResponseFacts() {
        // Keep track of the Fact names
        std::set<std::string> fact_names;

        // Iterate through the Consequent's list of Assignments
        for (auto& current_assignment: response_.assignments_) {
            // Process the raw Assignment data
            switch (current_assignment.type_) {
                case utility::ExpertSystemTypes::kBool: {
                    // Gather the raw Assignment and store its target Fact's name
                    auto& raw_assignment = std::get<BoolAssignment>(current_assignment.assignment_);
                    fact_names.insert(raw_assignment.fact_);
                }
                case utility::ExpertSystemTypes::kInt: {
                    // Gather the raw Assignment and store its target Fact's name
                    auto& raw_assignment = std::get<IntAssignment>(current_assignment.assignment_);
                    fact_names.insert(raw_assignment.fact_);
                }
                case utility::ExpertSystemTypes::kFloat: {
                    // Gather the raw Assignment and store its target Fact's name
                    auto& raw_assignment = std::get<FloatAssignment>(current_assignment.assignment_);
                    fact_names.insert(raw_assignment.fact_);
                }
                case utility::ExpertSystemTypes::kEnum: {
                    // Gather the raw Assignment and store its target Fact's name
                    auto& raw_assignment = std::get<EnumAssignment>(current_assignment.assignment_);
                    fact_names.insert(raw_assignment.fact_);
                }
                default: {
                    // Just skip this one
                    break;
                }
            }
        }

        // Return the set of Fact names
        return fact_names;
    }

    std::pair<TestOutcome, utility::Confidence> Rule::Run(facts::FactDatabase& database,
                                                          std::string source) {
        // Run the Antecedent to identify if the Consequent should be triggered
        auto result = trigger_.Test(database);

        // Run the Consequent only if the Antecedent was triggered
        if (result.first == TestOutcome::kComparisonSuccess) {
            // Run the Consequent
            response_.Assign(database, result.second);
        }

        // TODO: Log the generation of this session value.

        // Return the Antecedent's result
        return result;
    }

    void Rule::SetDescription(std::string description) {
        // Trim the whitespace from the front of the description
        description.erase(description.begin(),
                          std::find_if(description.begin(), description.end(),
                                       [](unsigned char ch) { return !std::isspace(ch); }));

        // Trim the whitespace from the back of the description
        description.erase(
                std::find_if(description.rbegin(), description.rend(),
                             [](unsigned char ch) { return !std::isspace(ch); })
                        .base(),
                description.end());

        // Store the trimmed description string
        description_ = description;
    }

    std::string Rule::GetDescription() {
        // Simply provide a copy of the stored string
        return description_;
    }

} // namespace expert_system::knowledge::rules
