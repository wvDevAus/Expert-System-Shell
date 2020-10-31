#pragma once

#include "nlohmann/json.hpp"

namespace expert_system::utility {

        /// The symbolic representations of the compatible types for the expert system shell.
    enum class ExpertSystemTypes {
        kUnknown,
        kBool,
        kInt,
        kFloat,
        kEnum
    };

        /// JSON serialization for the ExpertSystemTypes enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(ExpertSystemTypes,
                                {{ExpertSystemTypes::kUnknown, nullptr},
                                 {ExpertSystemTypes::kBool, "Boolean"},
                                 {ExpertSystemTypes::kInt, "Integer"},
                                 {ExpertSystemTypes::kFloat, "Float"},
                                 {ExpertSystemTypes::kEnum, "Enumeration"}})

} // namespace expert_system::utility
