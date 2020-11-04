#pragma once

#include <optional>
#include <string>
#include <variant>

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/facts/Facts.hpp"

namespace expert_system::knowledge::rules {

        /**
         * @brief Template Assignment.
         * @tparam T The raw type used in the Assignment.
         */
    template <class T>
    struct Assignment {
            /// Default constructor.
        Assignment() = default;

            /// The identifying name of the target Fact.
        std::string fact_;

            /// The value to assign to the target Fact.
        T value_;

            /// The Confidence factor or the value to be assigned.
        utility::Confidence confidence_factor_;

            /**
             * @brief Attempts to perform the Assignment.
             * @param [in] database The Fact Database to operate on.
             * @return True if the target Fact was assigned a session value, False otherwise.
             */
        bool Assign(facts::FactDatabase& database) {
            // Non-specialized types are not supported
            return false;
        }
    };

        /// Boolean specialization overload of templated Assignment::Assign.
    template<>
    bool Assignment<bool>::Assign(facts::FactDatabase& database) {
        // Confirm the identified Fact actually exists
        auto optional_fact = database.Find(fact_);
        if (!optional_fact.has_value()) {
            // Stop and indicate failure
            return false;
        }

        // Gather the raw Fact by reference
        auto& variant_fact = optional_fact.value().get();
        if (variant_fact.type_ != utility::ExpertSystemTypes::kBool) {
            // Catch type mismatch and indicate failure
            return false;
        }

        // Attempt to assign the Value
        auto& raw_fact = std::get<facts::BoolFact>(variant_fact.fact_);
        return raw_fact.SetValue(value_, confidence_factor_);
    }

        /// Integer specialization overload of templated Assignment::Assign.
    template<>
    bool Assignment<int>::Assign(facts::FactDatabase& database) {
        // Confirm the identified Fact actually exists
        auto optional_fact = database.Find(fact_);
        if (!optional_fact.has_value()) {
            // Stop and indicate failure
            return false;
        }

        // Gather the raw Fact by reference
        auto& variant_fact = optional_fact.value().get();
        if (variant_fact.type_ != utility::ExpertSystemTypes::kInt) {
            // Catch type mismatch and indicate failure
            return false;
        }

        // Attempt to assign the Value
        auto& raw_fact = std::get<facts::IntFact>(variant_fact.fact_);
        return raw_fact.SetValue(value_, confidence_factor_);
    }

        /// Float specialization overload of templated Assignment::Assign.
    template<>
    bool Assignment<float>::Assign(facts::FactDatabase& database) {
        // Confirm the identified Fact actually exists
        auto optional_fact = database.Find(fact_);
        if (!optional_fact.has_value()) {
            // Stop and indicate failure
            return false;
        }

        // Gather the raw Fact by reference
        auto& variant_fact = optional_fact.value().get();
        if (variant_fact.type_ != utility::ExpertSystemTypes::kFloat) {
            // Catch type mismatch and indicate failure
            return false;
        }

        // Attempt to assign the Value
        auto& raw_fact = std::get<facts::FloatFact>(variant_fact.fact_);
        return raw_fact.SetValue(value_, confidence_factor_);
    }

        /// Enumeration specialization overload of templated Assignment::Assign.
    template<>
    bool Assignment<std::string>::Assign(facts::FactDatabase& database) {
        // Confirm the identified Fact actually exists
        auto optional_fact = database.Find(fact_);
        if (!optional_fact.has_value()) {
            // Stop and indicate failure
            return false;
        }

        // Gather the raw Fact by reference
        auto& variant_fact = optional_fact.value().get();
        if (variant_fact.type_ != utility::ExpertSystemTypes::kEnum) {
            // Catch type mismatch and indicate failure
            return false;
        }

        // Attempt to assign the Value
        auto& raw_fact = std::get<facts::EnumFact>(variant_fact.fact_);
        auto enum_conversion = raw_fact.enum_.At(value_);
        if (enum_conversion == std::nullopt) {
            // Catch the enum value not existing
            return false;
        }
        return raw_fact.fact_.SetValue(enum_conversion.value(), confidence_factor_);
    }

} // namespace expert_system::knowledge::rules
