#include "Consequent.hpp"

namespace expert_system::knowledge::rules {

    std::map<std::string, bool> Consequent::Assign(facts::FactDatabase& database) {
        // Keep track of the assignment outcomes
        std::map<std::string, bool> outcomes;

        // Iterate through the stored Assignments
        for (auto& current_assignment: assignments_) {
            // Process the specific type of stored Assignment
            switch (current_assignment.type_) {
                case utility::ExpertSystemTypes::kBool: {
                    // Get the raw assignment
                    auto& raw_assignment = std::get<BoolAssignment>(current_assignment.assignment_);

                    // Attempt the assignment and track the result
                    outcomes.emplace(raw_assignment.fact_,
                                     raw_assignment.Assign(database));
                    break;
                }
                case utility::ExpertSystemTypes::kInt: {
                    // Get the raw assignment
                    auto& raw_assignment = std::get<IntAssignment>(current_assignment.assignment_);

                    // Attempt the assignment and track the result
                    outcomes.emplace(raw_assignment.fact_,
                                     raw_assignment.Assign(database));
                    break;
                }
                case utility::ExpertSystemTypes::kFloat: {
                    // Get the raw assignment
                    auto& raw_assignment = std::get<FloatAssignment>(current_assignment.assignment_);

                    // Attempt the assignment and track the result
                    outcomes.emplace(raw_assignment.fact_,
                                     raw_assignment.Assign(database));
                    break;
                }
                case utility::ExpertSystemTypes::kEnum: {
                    // Get the raw assignment
                    auto& raw_assignment = std::get<EnumAssignment>(current_assignment.assignment_);

                    // Attempt the assignment and track the result
                    outcomes.emplace(raw_assignment.fact_,
                                     raw_assignment.Assign(database));
                    break;
                }
                default: {
                    // Just skip this Assignment
                    break;
                }
            }
        }

        // Return the outcomes
        return outcomes;
    }

    void to_json(nlohmann::json& json_sys, const Consequent& target) {
        // Export the list of Assignments
        json_sys = target.assignments_;
    }

    void from_json(const nlohmann::json& json_sys, Consequent& target) {
        // Attempt to gather the Assignments
        if (!json_sys.empty()) {
            // Store the list of Assignments
            target.assignments_ = json_sys.get<std::list<VariantAssignment>>();
        }
    }

} // namespace expert_system::knowledge::rules
