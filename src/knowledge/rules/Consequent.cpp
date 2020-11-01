#include "Consequent.hpp"

namespace expert_system::knowledge::rules {

    void Consequent::Assign(facts::FactDatabase& database,
                            utility::Confidence& confidence_factor) {
        // Iterate through the stored Assignments
        for (auto& current_assignment: assignments_) {
            // Process the specific type of stored Assignment
            switch (current_assignment.type_) {
                case utility::ExpertSystemTypes::kBool: {
                    auto& raw_assignment = std::get<BoolAssignment>(current_assignment.assignment_);
                    raw_assignment.Assign(database, confidence_factor);
                    break;
                }
                case utility::ExpertSystemTypes::kInt: {
                    auto& raw_assignment = std::get<IntAssignment>(current_assignment.assignment_);
                    raw_assignment.Assign(database, confidence_factor);
                    break;
                }
                case utility::ExpertSystemTypes::kFloat: {
                    auto& raw_assignment = std::get<FloatAssignment>(current_assignment.assignment_);
                    raw_assignment.Assign(database, confidence_factor);
                    break;
                }
                case utility::ExpertSystemTypes::kEnum: {
                    auto& raw_assignment = std::get<EnumAssignment>(current_assignment.assignment_);
                    raw_assignment.Assign(database, confidence_factor);
                    break;
                }
                default: {
                    // Just skip this Assignment
                    break;
                }
            }
        }
    }

} // namespace expert_system::knowledge::rules
