#include "Values.hpp"

namespace expert_system::knowledge::facts {

    EnumValue::EnumValue(const utility::DynamicEnum& value_enum, IntValue value_data)
        : enum_(value_enum), value_(value_data) {}

    VariantValue::VariantValue()
        : type_(utility::ExpertSystemTypes::kUnknown),
          value_(std::monostate()) {};

    VariantValue::VariantValue(VariantFact value) {
        // Store the type
        type_ = value.type_;

        // Split the logic based on the type
        switch (type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Catch if the provided Fact actually has a session value
                auto raw_value = std::get<BoolFact>(value.fact_).GetValue();
                if (!raw_value.has_value()) {
                    // Don't store any data
                    type_ = utility::ExpertSystemTypes::kUnknown;
                    value_ = std::monostate();
                    break;
                }

                // Copy the stored Value
                value_ = raw_value.value();
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Catch if the provided Fact actually has a session value
                auto raw_value = std::get<IntFact>(value.fact_).GetValue();
                if (!raw_value.has_value()) {
                    // Don't store any data
                    type_ = utility::ExpertSystemTypes::kUnknown;
                    value_ = std::monostate();
                    break;
                }

                // Copy the stored Value
                value_ = raw_value.value();
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Catch if the provided Fact actually has a session value
                auto raw_value = std::get<FloatFact>(value.fact_).GetValue();
                if (!raw_value.has_value()) {
                    // Don't store any data
                    type_ = utility::ExpertSystemTypes::kUnknown;
                    value_ = std::monostate();
                    break;
                }

                // Copy the stored Value
                value_ = raw_value.value();
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Catch if the provided Fact actually has a session value
                auto raw_fact = std::get<EnumFact>(value.fact_);
                if (!raw_fact.fact_.GetValue().has_value()) {
                    // Don't store any data
                    type_ = utility::ExpertSystemTypes::kUnknown;
                    value_ = std::monostate();
                    break;
                }

                // Copy the stored Value
                value_ = EnumValue(raw_fact.enum_, raw_fact.fact_.GetValue().value());
                break;
            }
            default: {
                // Don't store any data
                type_ = utility::ExpertSystemTypes::kUnknown;
                value_ = std::monostate();
                break;
            }
        }
    }

} // namespace expert_system::knowledge::facts
