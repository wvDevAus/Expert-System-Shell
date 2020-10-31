#pragma once

#include <optional>
#include <variant>

#include "knowledge/facts/Fact.hpp"
#include "utility/DynamicEnum.hpp"

namespace expert_system::knowledge::facts {

        /// The symbolic representations of the compatible types of Facts.
    enum class FactType {
        kUnknown,
        kBoolFact,
        kIntFact,
        kFloatFact,
        kEnumFact
    };

        /// JSON serialization for the FactType enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(FactType,
                                {{FactType::kUnknown, nullptr},
                                 {FactType::kBoolFact, "Boolean"},
                                 {FactType::kIntFact, "Integer"},
                                 {FactType::kFloatFact, "Float"},
                                 {FactType::kEnumFact, "Enumeration"}})

        /// Boolean specialization of the Fact template
    using BoolFact = Fact<bool>;

        /// Integer specialization of the Fact template
    using IntFact = Fact<int>;

        /// Float specialization of the Fact template
    using FloatFact = Fact<float>;

        /// Enum 'proxy'/'specialization' of the Fact template
    struct EnumFact {
            /**
             * @brief Default constructor.
             * Constructs both members using their default constructors.
             */
        EnumFact() = default;

            /**
             * @brief Parameterized constructor, assigns starting enums values.
             * Constructs the Fact with its default parameters.
             * @param [in] ordered_names A list of enum names in ascending order.
             * @note Duplicate enum names will be discarded.
             */
        explicit EnumFact(const std::list<std::string>& ordered_names);

            /**
             * @brief The enum values for the Fact, dynamically configurable.
             * @warning Avoid editing this while using the Fact!
             */
        utility::DynamicEnum enum_;

            /**
             * @brief The raw Fact, interfacing to the DynamicEnum through an int specialization.
             * @warning The contents of this may be invalidated if the enum values are edited!
             */
        IntFact fact_;
    };

        /// Type-safe union of the four Fact types
    using GenericFact = std::variant<std::monostate, BoolFact, IntFact, FloatFact, EnumFact>;

        /// Type-tracked Generic Fact
    struct VariantFact {
            /// Default constructor.
        VariantFact();

            /**
             * Parameterized constructor, generates a Fact of a specified type.
             * @param [in] type The type of Fact to generate and store.
             */
        explicit VariantFact(FactType type);

            /**
             * @brief A hint for the type of Fact that is stored.
             * @warning This may be incorrect, and should be confirmed before use of the Fact!
             */
        FactType type_;

            /// A generic type of Fact.
        GenericFact fact_;
    };

        /**
         * @brief VariantFact serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the VariantFact to export.
         */
    void to_json(nlohmann::json& json_sys, const VariantFact& target);

        /**
         * @brief VariantFact serialization from JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the VariantFact to export.
         */
    void from_json(const nlohmann::json& json_sys, VariantFact& target);

} // namespace expert_system::knowledge::facts
