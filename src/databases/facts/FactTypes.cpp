#include "FactTypes.hpp"

namespace expert_system {

    EnumFact::EnumFact(std::list<std::string> ordered_names) {
        // Call the DynamicEnum's parameterized constructor
        enum_ = DynamicEnum(ordered_names);
    }

    VariantFact::VariantFact(FactType type) {
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
        }

        // Store the hint for the generated type
        type_ = type;
    }

} // namespace expert_system
