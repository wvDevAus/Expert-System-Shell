#pragma once

#include <string>

#include "knowledge/rules/Condition.hpp"
#include "utility/Types.hpp"

namespace expert_system::knowledge::rules {

        /// Boolean specialization of the Condition template.
    using BoolCondition = Condition<bool>;

        /// Integer specialization of the Condition template.
    using IntCondition = Condition<int>;

        /// Float specialization of the Condition template.
    using FloatCondition = Condition<float>;

        /// DynamicEnum specialization of the Condition template.
    using EnumCondition = Condition<std::string>;

        /// Type-safe union of the four Condition types
    using GenericCondition = std::variant<std::monostate, BoolCondition,
                                     IntCondition, FloatCondition, EnumCondition>;

        /// Type-tracked Generic Condition
    struct VariantCondition {
            /**
             * @brief Default constructor.
             * Sets type_ to kUnknown and condition_ to std::monostate.
             */
        VariantCondition();

            /**
             * Parameterized constructor, generates a Condition of a specified type.
             * @param [in] type The type of Condition to generate and store.
             */
        explicit VariantCondition(utility::ExpertSystemTypes type);

            /**
             * @brief Tests the encapsulated Condition.
             * @param source The Fact Database to operate on.
             * @return A TestOutcome enum symbol indicating the test's result.
             * @note This will return TestOutcome::kUnknown if no Condition is stored.
             */
        TestOutcome Test(facts::FactDatabase& source);

            /**
             * @brief Provides read-only access to the encapsulated Condition's Fact identifier.
             * @return A copy of the encapsulated Condition's Fact identifier.
             * @note This will return std::nullopt if no Condition is stored.
             */
        std::optional<std::string> Fact();

            /**
             * @brief Provides read-only access to the encapsulated Condition's target.
             * @return A copy of the encapsulated Condition's target identifier.
             * @note This will return std::nullopt if no Condition is stored.
             */
        std::optional<ConditionType> Condition();

            /**
             * @brief A hint for the type of Fact that is stored.
             * @warning This may be incorrect, and should be confirmed before use of the Fact!
             */
        utility::ExpertSystemTypes type_;

            /// The encapsulated condition
        GenericCondition condition_;
    };

} // namespace expert_system::knowledge::rules
