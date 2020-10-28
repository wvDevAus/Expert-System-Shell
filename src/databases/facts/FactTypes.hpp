#pragma once

#include <optional>
#include <variant>

#include "databases/facts/Fact.hpp"
#include "utility/DynamicEnum.hpp"

namespace expert_system {

        /// Contains symbols
    enum class FactType {
        kBoolFact,
        kIntFact,
        kFloatFact,
        kEnumFact
    };

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
        EnumFact();

            /**
             * @brief Parameterized constructor, assigns starting enums values.
             * Constructs the Fact with its default parameters.
             * @param [in] ordered_names A list of enum names in ascending order.
             * @note Duplicate enum names will be discarded.
             */
        EnumFact(std::list<std::string> ordered_names);

            /**
             * @brief The enum values for the Fact, dynamically configurable.
             * @warning Avoid editing this while using the Fact!
             */
        DynamicEnum enum_;

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
            /// Use of the default constructor is not allowed!
        VariantFact() = delete;

            /**
             * Parameterized constructor, generates a Fact of a specified type.
             * @param type The type of Fact to generate and store.
             */
        VariantFact(FactType type);

            /**
             * @brief A hint for the type of Fact that is stored.
             * @warning This may be incorrect, and should be confirmed before use of the Fact!
             */
        FactType type_;

            /// A generic type of Fact.
        GenericFact fact_;
    };

} // namespace expert_system
