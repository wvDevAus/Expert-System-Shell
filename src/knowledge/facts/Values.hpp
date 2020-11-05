#pragma once

#include <variant>

#include "knowledge/facts/Facts.hpp"
#include "knowledge/facts/Value.hpp"
#include "utility/DynamicEnum.hpp"
#include "utility/Types.hpp"

namespace expert_system::knowledge::facts {

        /// Boolean specialization of the Value template.
    using BoolValue = knowledge::facts::Value<bool>;

        /// Integer specialization of the Value template.
    using IntValue = knowledge::facts::Value<int>;

        /// Float specialization of the Value template.
    using FloatValue = knowledge::facts::Value<float>;

        /// Enum 'proxy'/'specialization' of the Value template.
    struct EnumValue {
            /**
             * @brief Parameterized constructor, assigns starting enums values.
             * Constructs the Fact with its default parameters.
             * @param [in] ordered_names A list of enum names in ascending order.
             * @note Duplicate enum names will be discarded.
             */
        explicit EnumValue(const utility::DynamicEnum& value_enum);

            /// The DynamicEnum for use in converting the Value data.
        utility::DynamicEnum enum_;

            /// The raw Value, stored using integer DynamicEnum index values.
        knowledge::facts::Value<int> value_;
    };

        /// Type-safe union of the four Value types
    using GenericValue = std::variant<std::monostate, BoolValue, IntValue, FloatValue, EnumValue>;

        /// Type-tracked Generic Fact
    struct VariantValue {
            /**
             * @brief Default constructor.
             * Sets type_ to kUnknown and value_ to std::monostate.
             */
        VariantValue();

            /**
             * Parameterized constructor.
             * Copies the Value from an existing Fact and automatically determines its type.
             * @param [in] value The Fact to gather a Value from.
             */
        explicit VariantValue(VariantFact value);

            /**
             * @brief A hint for the type of Value that is stored.
             * @warning This may be incorrect, and should be confirmed before use of the Value!
             */
        utility::ExpertSystemTypes type_;

            /// The encapsulated Value.
        GenericValue value_;
    };

} // namespace expert_system::knowledge::facts
