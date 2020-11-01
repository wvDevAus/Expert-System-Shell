#pragma once

#include <string>
#include <variant>

#include "knowledge/rules/Assignment.hpp"

namespace expert_system::knowledge::rules {

        /// Boolean specialization of the Assignment template.
    using BoolAssignment = Assignment<bool>;

        /// Integer specialization of the Assignment template.
    using IntAssignment = Assignment<int>;

        /// Float specialization of the Assignment template.
    using FloatAssignment = Assignment<float>;

        /// Enumeration specialization of the Assignment template.
    using EnumAssignment = Assignment<std::string>;

        /// Type-safe union of the four Assignment types
    using GenericAssignment = std::variant<std::monostate, BoolAssignment,
                                          IntAssignment, FloatAssignment, EnumAssignment>;

        /// Type-tracked Generic Assignment
    struct VariantAssignment {
            /**
             * @brief Default constructor.
             * Sets type_ to kUnknown and assignment_ to std::monostate.
             */
        VariantAssignment();

            /**
             * Parameterized constructor, generates an Assignment of a specified type.
             * @param [in] type The type of Assignment to generate and store.
             */
        explicit VariantAssignment(utility::ExpertSystemTypes type);

            /**
             * @brief A hint for the type of Assignment that is stored.
             * @warning This may be incorrect, and should be confirmed before use of the Fact!
             */
        utility::ExpertSystemTypes type_;

            /// The encapsulated Assignment
        GenericAssignment assignment_;
    };

        /**
         * @brief VariantAssignment serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the VariantAssignment to export.
         */
    void to_json(nlohmann::json& json_sys, const VariantAssignment& target);

        /**
         * @brief VariantFact serialization from JSON format.
         * @param [in] json_sys A reference to a JSON object.
         * @param [in,out] target A reference to the VariantAssignment to import.
         */
    void from_json(const nlohmann::json& json_sys, VariantAssignment& target);

} // namespace expert_system::knowledge::rules
