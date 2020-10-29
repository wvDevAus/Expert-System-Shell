#include "FactTypes.hpp"

namespace expert_system {

    EnumFact::EnumFact(std::list<std::string> ordered_names) {
        // Call the DynamicEnum's parameterized constructor
        enum_ = DynamicEnum(ordered_names);
    }

    VariantFact::VariantFact() : type_(FactType::kUnknown) {}

    VariantFact::VariantFact(FactType type) {
        // Store the hint for the generated type
        type_ = type;

        // Generate and store a specific Fact specialization
        switch (type) {
            case FactType::kBoolFact:
                fact_ = BoolFact();
                break;
            case FactType::kIntFact:
                fact_ = IntFact();
                break;
            case FactType::kFloatFact:
                fact_ = FloatFact();
                break;
            case FactType::kEnumFact:
                fact_ = EnumFact();
                break;
            default:
                fact_ = std::monostate();
                type_ = FactType::kUnknown;
                break;
        }
    }

    /*void to_json(nlohmann::json& json_sys, const VariantFact& target) {

    }*/

    /*void from_json(const nlohmann::json& json_sys, VariantFact& target) {

    }*/

} // namespace expert_system
