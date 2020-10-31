#pragma once

#include "nlohmann/json.hpp"

namespace expert_system::knowledge::rules {

        /// The symbolic representations of the compatible Antecedent conditions.
    enum class AntecedentType {
        kUnknown,
        kEqualTo,
        kLessThan,
        kGreaterThan
    };

        /// JSON serialization for the AntecedentType enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(AntecedentType,
                                {{AntecedentType::kUnknown, nullptr},
                                 {AntecedentType::kEqualTo, "EqualTo"},
                                 {AntecedentType::kLessThan, "GreaterThan"},
                                 {AntecedentType::kGreaterThan, "LessThan"}})

} // namespace expert_system::knowledge::rules
