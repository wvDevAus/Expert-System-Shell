#include "Assignments.hpp"

#include "utility/SymbolsJSON.hpp"

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

    void to_json(nlohmann::json& json_sys, const VariantAssignment& target) {
        // Store the type
        json_sys[utility::JSON_ID_TYPE] = target.type_;

        // Split logic depending on the current type
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Get the raw Assignment type
                auto assignment_raw = std::get<BoolAssignment>(target.assignment_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = assignment_raw.fact_;

                // Export the target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = assignment_raw.value_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Get the raw Assignment type
                auto assignment_raw = std::get<IntAssignment>(target.assignment_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = assignment_raw.fact_;

                // Export the target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = assignment_raw.value_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Get the raw Assignment type
                auto assignment_raw = std::get<FloatAssignment>(target.assignment_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = assignment_raw.fact_;

                // Export the target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = assignment_raw.value_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Get the raw Assignment type
                auto assignment_raw = std::get<EnumAssignment>(target.assignment_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = assignment_raw.fact_;

                // Export the target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = assignment_raw.value_;

                // Done
                break;
            }
            default: {
                // Assign no more data
                break;
            }
        }
    }

    void from_json(const nlohmann::json& json_sys, VariantAssignment& target) {
        // Clear the existing Assignment data
        target.type_ = utility::ExpertSystemTypes::kUnknown;
        target.assignment_ = std::monostate();

        // Check if the type data is stored
        if (json_sys.find(utility::JSON_ID_TYPE) != json_sys.end()) {
            // Attempt to gather the type
            if (json_sys.at(utility::JSON_ID_TYPE).is_string()) {
                // Store the type
                target.type_ = json_sys.at(utility::JSON_ID_TYPE).get<utility::ExpertSystemTypes>();
            }
        }

        // Process the Assignment data
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Create and track an empty BoolAssignment
                target.assignment_.emplace<BoolAssignment>();
                auto& target_assignment = std::get<BoolAssignment>(target.assignment_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_assignment.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_boolean()) {
                        // Store the target value
                        target_assignment.value_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<bool>();
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Create and track an empty IntAssignment
                target.assignment_.emplace<IntAssignment>();
                auto& target_assignment = std::get<IntAssignment>(target.assignment_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_assignment.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_number_integer()) {
                        // Store the target value
                        target_assignment.value_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<int>();
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Create and track an empty FloatAssignment
                target.assignment_.emplace<FloatAssignment>();
                auto& target_assignment = std::get<FloatAssignment>(target.assignment_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_assignment.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_number_float()) {
                        // Store the target value
                        target_assignment.value_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<float>();
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Create and track an empty EnumAssignment
                target.assignment_.emplace<EnumAssignment>();
                auto& target_assignment = std::get<EnumAssignment>(target.assignment_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_assignment.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_string()) {
                        // Store the target value
                        target_assignment.value_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<std::string>();
                    }
                }

                // Finished
                break;
            }
            default: {
                // Don't assign any more data
                break;
            }
        }
    }

} // namespace expert_system::knowledge::rules
