#include "Assignments.hpp"

namespace expert_system::knowledge::rules {

    VariantAssignment::VariantAssignment()
            : type_(utility::ExpertSystemTypes::kUnknown),
              assignment_(std::monostate()) {}

    VariantAssignment::VariantAssignment(utility::ExpertSystemTypes type) {
        // Store the type
        type_ = type;

        // Generate and store a specific Assignment specialization
        switch (type) {
            case utility::ExpertSystemTypes::kBool: {
                assignment_ = BoolAssignment();
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                assignment_ = IntAssignment();
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                assignment_ = FloatAssignment();
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                assignment_ = EnumAssignment();
                break;
            }
            default: {
                assignment_ = std::monostate();
                type_ = utility::ExpertSystemTypes::kUnknown;
                break;
            }
        }
    }

} // namespace expert_system::knowledge::rules
