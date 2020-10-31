#include "Conditions.hpp"

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

} // namespace expert_system::knowledge::rules
