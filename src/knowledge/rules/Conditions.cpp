#include "Conditions.hpp"

#include "utility/SymbolsJSON.hpp"

namespace expert_system::knowledge::rules {

    VariantCondition::VariantCondition() : type_(utility::ExpertSystemTypes::kUnknown),
                                           condition_(std::monostate()) {};

    VariantCondition::VariantCondition(utility::ExpertSystemTypes type) {
        // Store the hint for the generated type
        type_ = type;

        // Generate and store a specific Condition specialization
        switch (type) {
            case utility::ExpertSystemTypes::kBool: {
                condition_ = BoolCondition();
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                condition_ = IntCondition();
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                condition_ = FloatCondition();
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                condition_ = EnumCondition();
                break;
            }
            default: {
                condition_ = std::monostate();
                type_ = utility::ExpertSystemTypes::kUnknown;
                break;
            }
        }
    }

    TestOutcome VariantCondition::Test(facts::FactDatabase& source) {
        // Attempt to gather the raw Condition
        switch (type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<BoolCondition>(condition_);

                // Return the test result
                return raw_condition.Test(source);
            }
            case utility::ExpertSystemTypes::kInt: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<IntCondition>(condition_);

                // Return the test result
                return raw_condition.Test(source);
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<FloatCondition>(condition_);

                // Return the test result
                return raw_condition.Test(source);
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<EnumCondition>(condition_);

                // Return the test result
                return raw_condition.Test(source);
            }
            default: {
                // Indicate failure
                return TestOutcome::kUnknown;
            }
        }
    }

    std::optional<std::string> VariantCondition::Fact() {
        // Attempt to gather the raw Condition
        switch (type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<BoolCondition>(condition_);

                // Return the target
                return raw_condition.fact_;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<IntCondition>(condition_);

                // Return the target
                return raw_condition.fact_;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<FloatCondition>(condition_);

                // Return the target
                return raw_condition.fact_;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<EnumCondition>(condition_);

                // Return the target
                return raw_condition.fact_;
            }
            default: {
                // Indicate failure
                return std::nullopt;
            }
        }
    }

    std::optional<ConditionType> VariantCondition::Condition() {
        // Attempt to gather the raw Condition
        switch (type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<BoolCondition>(condition_);

                // Return the type of condition it would perform when testing
                return raw_condition.condition_;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<IntCondition>(condition_);

                // Return the type of condition it would perform when testing
                return raw_condition.condition_;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<FloatCondition>(condition_);

                // Return the type of condition it would perform when testing
                return raw_condition.condition_;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<EnumCondition>(condition_);

                // Return the type of condition it would perform when testing
                return raw_condition.condition_;
            }
            default: {
                // Indicate failure
                return std::nullopt;
            }
        }
    }

    std::optional<bool> VariantCondition::Invert() {
        // Attempt to gather the raw Condition
        switch (type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<BoolCondition>(condition_);

                // Return the inversion flag
                return raw_condition.invert_;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<IntCondition>(condition_);

                // Return the inversion flag
                return raw_condition.invert_;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<FloatCondition>(condition_);

                // Return the inversion flag
                return raw_condition.invert_;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Store a reference to the Condition
                auto& raw_condition = std::get<EnumCondition>(condition_);

                // Return the inversion flag
                return raw_condition.invert_;
            }
            default: {
                // Indicate failure
                return std::nullopt;
            }
        }
    }

    void to_json(nlohmann::json& json_sys, const VariantCondition& target) {
        // Assign the type
        json_sys[utility::JSON_ID_TYPE] = target.type_;

        // Split logic depending on the current type
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Get the raw Condition
                auto raw_condition = std::get<BoolCondition>(target.condition_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = raw_condition.fact_;

                // Export the Condition's condition type
                json_sys[utility::JSON_ID_CONDITION] = raw_condition.condition_;

                // Export the Condition's target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = raw_condition.target_;

                // Export the Condition's inversion flag
                json_sys[utility::JSON_ID_INVERT] = raw_condition.invert_;

                // Export the Condition's confidence factor
                json_sys[utility::JSON_ID_CONFIDENCE] = raw_condition.confidence_factor_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Get the raw Condition
                auto raw_condition = std::get<IntCondition>(target.condition_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = raw_condition.fact_;

                // Export the Condition's condition type
                json_sys[utility::JSON_ID_CONDITION] = raw_condition.condition_;

                // Export the Condition's target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = raw_condition.target_;

                // Export the Condition's inversion flag
                json_sys[utility::JSON_ID_INVERT] = raw_condition.invert_;

                // Export the Condition's confidence factor
                json_sys[utility::JSON_ID_CONFIDENCE] = raw_condition.confidence_factor_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Get the raw Condition
                auto raw_condition = std::get<FloatCondition>(target.condition_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = raw_condition.fact_;

                // Export the Condition's condition type
                json_sys[utility::JSON_ID_CONDITION] = raw_condition.condition_;

                // Export the Condition's target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = raw_condition.target_;

                // Export the Condition's inversion flag
                json_sys[utility::JSON_ID_INVERT] = raw_condition.invert_;

                // Export the Condition's confidence factor
                json_sys[utility::JSON_ID_CONFIDENCE] = raw_condition.confidence_factor_;

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Get the raw Condition
                auto raw_condition = std::get<EnumCondition>(target.condition_);

                // Export the target Fact
                json_sys[utility::JSON_ID_FACT] = raw_condition.fact_;

                // Export the Condition's condition type
                json_sys[utility::JSON_ID_CONDITION] = raw_condition.condition_;

                // Export the Condition's target value
                json_sys[utility::JSON_ID_TARGET_VALUE] = raw_condition.target_;

                // Export the Condition's inversion flag
                json_sys[utility::JSON_ID_INVERT] = raw_condition.invert_;

                // Export the Condition's confidence factor
                json_sys[utility::JSON_ID_CONFIDENCE] = raw_condition.confidence_factor_;

                // Done
                break;
            }
            default: {
                // Don't assign any more data
                break;
            }
        }
    }

    void from_json(const nlohmann::json& json_sys, VariantCondition& target) {
        // Clear the existing Condition data
        target.condition_ = std::monostate();
        target.type_ = utility::ExpertSystemTypes::kUnknown;

        // Check if the type data is stored
        if (json_sys.find(utility::JSON_ID_TYPE) != json_sys.end()) {
            // Attempt to gather the type
            if (json_sys.at(utility::JSON_ID_TYPE).is_string()) {
                // Store the type
                target.type_ = json_sys.at(utility::JSON_ID_TYPE).get<utility::ExpertSystemTypes>();
            }
        }

        // Process the Condition data
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Create and track an empty BoolCondition
                target.condition_.emplace<BoolCondition>();
                auto& target_condition = std::get<BoolCondition>(target.condition_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_condition.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the condition type
                if (json_sys.find(utility::JSON_ID_CONDITION) != json_sys.end()) {
                    // Attempt to gather the condition type
                    if (json_sys.at(utility::JSON_ID_CONDITION).is_string()) {
                        // Store the condition type
                        target_condition.condition_ = json_sys.at(utility::JSON_ID_CONDITION).get<ConditionType>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_boolean()) {
                        // Store the target value
                        target_condition.target_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<bool>();
                    }
                }

                // Attempt to find the inversion flag
                if (json_sys.find(utility::JSON_ID_INVERT) != json_sys.end()) {
                    // Attempt to gather the inversion flag
                    if (json_sys.at(utility::JSON_ID_INVERT).is_boolean()) {
                        // Store the inversion flag
                        target_condition.invert_ = json_sys.at(utility::JSON_ID_INVERT).get<bool>();
                    }
                }

                // Attempt to get the confidence factor
                if (json_sys.find(utility::JSON_ID_CONFIDENCE) != json_sys.end()) {
                    // Attempt to gather the confidence factor
                    if (json_sys.at(utility::JSON_ID_CONFIDENCE).is_number_float()) {
                        // Store the confidence factor
                        target_condition.confidence_factor_ = json_sys.at(utility::JSON_ID_CONFIDENCE).get<utility::Confidence>();
                    }
                }

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Create and track an empty IntCondition
                target.condition_.emplace<IntCondition>();
                auto& target_condition = std::get<IntCondition>(target.condition_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_condition.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the condition type
                if (json_sys.find(utility::JSON_ID_CONDITION) != json_sys.end()) {
                    // Attempt to gather the condition type
                    if (json_sys.at(utility::JSON_ID_CONDITION).is_string()) {
                        // Store the condition type
                        target_condition.condition_ = json_sys.at(utility::JSON_ID_CONDITION).get<ConditionType>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_number_integer()) {
                        // Store the target value
                        target_condition.target_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<int>();
                    }
                }

                // Attempt to find the inversion flag
                if (json_sys.find(utility::JSON_ID_INVERT) != json_sys.end()) {
                    // Attempt to gather the inversion flag
                    if (json_sys.at(utility::JSON_ID_INVERT).is_boolean()) {
                        // Store the inversion flag
                        target_condition.invert_ = json_sys.at(utility::JSON_ID_INVERT).get<bool>();
                    }
                }

                // Attempt to get the confidence factor
                if (json_sys.find(utility::JSON_ID_CONFIDENCE) != json_sys.end()) {
                    // Attempt to gather the confidence factor
                    if (json_sys.at(utility::JSON_ID_CONFIDENCE).is_number_float()) {
                        // Store the confidence factor
                        target_condition.confidence_factor_ = json_sys.at(utility::JSON_ID_CONFIDENCE).get<utility::Confidence>();
                    }
                }

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Create and track an empty FloatCondition
                target.condition_.emplace<FloatCondition>();
                auto& target_condition = std::get<FloatCondition>(target.condition_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_condition.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the condition type
                if (json_sys.find(utility::JSON_ID_CONDITION) != json_sys.end()) {
                    // Attempt to gather the condition type
                    if (json_sys.at(utility::JSON_ID_CONDITION).is_string()) {
                        // Store the condition type
                        target_condition.condition_ = json_sys.at(utility::JSON_ID_CONDITION).get<ConditionType>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_number_float()) {
                        // Store the target value
                        target_condition.target_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<float>();
                    }
                }

                // Attempt to find the inversion flag
                if (json_sys.find(utility::JSON_ID_INVERT) != json_sys.end()) {
                    // Attempt to gather the inversion flag
                    if (json_sys.at(utility::JSON_ID_INVERT).is_boolean()) {
                        // Store the inversion flag
                        target_condition.invert_ = json_sys.at(utility::JSON_ID_INVERT).get<bool>();
                    }
                }

                // Attempt to get the confidence factor
                if (json_sys.find(utility::JSON_ID_CONFIDENCE) != json_sys.end()) {
                    // Attempt to gather the confidence factor
                    if (json_sys.at(utility::JSON_ID_CONFIDENCE).is_number_float()) {
                        // Store the confidence factor
                        target_condition.confidence_factor_ = json_sys.at(utility::JSON_ID_CONFIDENCE).get<utility::Confidence>();
                    }
                }

                // Done
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Create and track an empty EnumCondition
                target.condition_.emplace<EnumCondition>();
                auto& target_condition = std::get<EnumCondition>(target.condition_);

                // Attempt to find the target Fact
                if (json_sys.find(utility::JSON_ID_FACT) != json_sys.end()) {
                    // Attempt to gather the target Fact
                    if (json_sys.at(utility::JSON_ID_FACT).is_string()) {
                        // Store the target Fact
                        target_condition.fact_ = json_sys.at(utility::JSON_ID_FACT).get<std::string>();
                    }
                }

                // Attempt to find the condition type
                if (json_sys.find(utility::JSON_ID_CONDITION) != json_sys.end()) {
                    // Attempt to gather the condition type
                    if (json_sys.at(utility::JSON_ID_CONDITION).is_string()) {
                        // Store the condition type
                        target_condition.condition_ = json_sys.at(utility::JSON_ID_CONDITION).get<ConditionType>();
                    }
                }

                // Attempt to find the target value
                if (json_sys.find(utility::JSON_ID_TARGET_VALUE) != json_sys.end()) {
                    // Attempt to gather the target value
                    if (json_sys.at(utility::JSON_ID_TARGET_VALUE).is_string()) {
                        // Store the target value
                        target_condition.target_ = json_sys.at(utility::JSON_ID_TARGET_VALUE).get<std::string>();
                    }
                }

                // Attempt to find the inversion flag
                if (json_sys.find(utility::JSON_ID_INVERT) != json_sys.end()) {
                    // Attempt to gather the inversion flag
                    if (json_sys.at(utility::JSON_ID_INVERT).is_boolean()) {
                        // Store the inversion flag
                        target_condition.invert_ = json_sys.at(utility::JSON_ID_INVERT).get<bool>();
                    }
                }

                // Attempt to get the confidence factor
                if (json_sys.find(utility::JSON_ID_CONFIDENCE) != json_sys.end()) {
                    // Attempt to gather the confidence factor
                    if (json_sys.at(utility::JSON_ID_CONFIDENCE).is_number_float()) {
                        // Store the confidence factor
                        target_condition.confidence_factor_ = json_sys.at(utility::JSON_ID_CONFIDENCE).get<utility::Confidence>();
                    }
                }

                // Done
                break;
            }
            default: {
                // Stop and do nothing
                break;
            }
        }
    }

} // namespace expert_system::knowledge::rules
